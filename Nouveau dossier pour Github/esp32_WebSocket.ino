#include <WiFi.h>                // Bibliothèque WiFi pour ESP32
#include <WebSocketsServer.h>    // Serveur WebSocket

// Configuration WiFi
const char* ssid = "Livebox-9410";
const char* password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Variables globales
bool gameEnded = false;          // Indique si la partie est terminée

// Configuration des broches RX1/TX1 pour la communication série
#define RXD1 16
#define TXD1 17

void connectToWiFi();
void ensureWiFiConnection(); // Vérifie et force la reconnexion au WiFi
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void processIncomingSerialData();
void endGame();
void resetGameState();

void setup() {
  Serial.begin(115200);

  // Initialisation de la liaison série avec l'Arduino Méga
  Serial1.begin(9600, SERIAL_8N1, RXD1, TXD1); // Utilisez directement Serial1
  Serial.println("🔗 Liaison série avec Arduino Méga initialisée.");

  // Connexion au réseau WiFi
  connectToWiFi();

  // Démarrage du serveur WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  // Vérifie et force la reconnexion WiFi si nécessaire
  ensureWiFiConnection();

  // Gestion des événements WebSocket
  webSocket.loop();

  // Traitement des données série provenant de l'Arduino Méga
  processIncomingSerialData();
}

// Connexion initiale au réseau WiFi
void connectToWiFi() {
  Serial.println("🌐 Connexion au WiFi...");
  WiFi.begin(ssid, password);

  int retryCount = 0;
  const int maxRetries = 10;

  while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries) {
    delay(500); // Délai court uniquement au démarrage
    Serial.print(".");
    retryCount++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connecté !");
    Serial.print("Adresse IP : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Échec de connexion WiFi, redémarrage...");
    ESP.restart();
  }
}

// Vérifie et force la reconnexion WiFi si nécessaire
void ensureWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("🔌 WiFi déconnecté. Tentative de reconnexion...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      // Pas de delay ici pour ne pas bloquer la loop principale
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("✅ Reconnexion WiFi réussie !");
    } else {
      Serial.println("❌ Impossible de se reconnecter au WiFi.");
    }
  }
}

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    Serial.println("📨 Message reçu : " + message);

    if (message == "{\"type\":\"reset\"}") {
      resetGameState(); // Remise à zéro des points
      gameEnded = false; // Relance de la partie
      Serial.println("🚀 Nouvelle partie lancée après 'reset'");
    } else if (message == "{\"type\":\"stop\"}") {
      gameEnded = true; // Arrêt du jeu
      Serial.println("🛑 Programme arrêté après 'stop'");
    }
  } else if (type == WStype_CONNECTED) {
    Serial.printf("✅ Client %d connecté\n", num);
    if (gameEnded) {
      Serial.println("🚀 Redémarrage automatique après reconnexion...");
      resetGameState();
      gameEnded = false;
    }
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("❌ Client %d déconnecté\n", num);
  }
}

// Traitement des données série reçues de l'Arduino Méga
void processIncomingSerialData() {
  while (Serial1.available()) {
    String incomingData = Serial1.readStringUntil('\n'); // Lecture ligne par ligne
    incomingData.trim();

    if (incomingData.startsWith("J")) { // Vérifie si le format est correct pour un joueur
      int playerIndex = incomingData.substring(1, 2).toInt() - 1; // Extrait l'index du joueur
      String pointAndScore = incomingData.substring(4); // Extrait le point et le score sous forme de chaîne

      if (playerIndex >= 0 && playerIndex < 4) {
        // Affichage dans le moniteur série
        Serial.printf("📥 Reçu : J%d : %s\n", playerIndex + 1, pointAndScore.c_str());

        // Envoi immédiat des données via WebSocket
        String jsonMessage = "{\"playerIndex\": " + String(playerIndex) +
                             ", \"point\": " + pointAndScore.substring(0, pointAndScore.indexOf(':')) +
                             ", \"score\": " + pointAndScore.substring(pointAndScore.indexOf(':') + 1) + "}";
        webSocket.broadcastTXT(jsonMessage);
        Serial.println("📤 Données envoyées : " + jsonMessage);
      } else {
        Serial.println("⚠️ Donnée reçue avec un index de joueur invalide !");
      }
    } else if (incomingData == "FIN GAME") { // Vérifie si le message est "FIN GAME"
      Serial.println("📥 Reçu : FIN GAME");
      endGame(); // Appelle la fonction de fin de partie
    } else {
      Serial.println("⚠️ Format de donnée invalide reçu !");
    }
  }
}

// Envoi du message de fin de partie
void endGame() {
  String endMessage = "FIN GAME";
  webSocket.broadcastTXT(endMessage);
  Serial.println("📤 Données envoyées : " + endMessage);
  gameEnded = true;
}

// Réinitialisation des points pour une nouvelle partie
void resetGameState() {
  gameEnded = false;
  Serial.println("🔄 État du jeu réinitialisé.");
}