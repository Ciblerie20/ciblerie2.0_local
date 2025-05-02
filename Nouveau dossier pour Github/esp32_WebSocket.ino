#include <WiFi.h>                // Bibliothèque WiFi pour ESP32
#include <WebSocketsServer.h>    // Serveur WebSocket

// Configuration WiFi
const char* ssid = "Livebox-9410";
const char* password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Variables globales
bool gameEnded = false;          // Indique si la partie est terminée
int points[4] = {0, 0, 0, 0};    // Points des joueurs (J1, J2, J3, J4)
int scores[4] = {0, 0, 0, 0};    // Scores cumulés des joueurs (J1, J2, J3, J4)

// Configuration des broches RX1/TX1 pour la communication série
#define RXD1 16
#define TXD1 17

void connectToWiFi();
void ensureWiFiConnection(); // Vérifie et force la reconnexion au WiFi
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void processIncomingSerialData();
void endGame();
void resetGameState();
int calculerScore(int point); // Fonction pour convertir les points en scores

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
      resetGameState(); // Remise à zéro des points et scores
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

// Fonction pour convertir les points en scores
int calculerScore(int point) {
  switch (point) {
    case 5: return 5;
    case 10: return 5;
    case 15: return 10;
    case 25: return 15;
    case 50: return 25;
    default: return 0;
  }
}

// Traitement des données série reçues de l'Arduino Méga
void processIncomingSerialData() {
  while (Serial1.available()) {
    String incomingData = Serial1.readStringUntil('\n'); // Lecture ligne par ligne
    incomingData.trim();

    if (incomingData.startsWith("J")) { // Vérifie si le format est correct pour un joueur
      int playerIndex = incomingData.substring(1, 2).toInt() - 1; // Extrait l'index du joueur
      int point = incomingData.substring(4).toInt(); // Extrait le point

      if (playerIndex >= 0 && playerIndex < 4) {
        points[playerIndex] = point; // Met à jour le point
        scores[playerIndex] += calculerScore(point); // Met à jour le score cumulé

        // Affichage dans le moniteur série
        Serial.printf("📥 Reçu : J%d : %d : %d\n", playerIndex + 1, point, scores[playerIndex]);

        // Envoi immédiat des données via WebSocket
        String jsonMessage = "{\"playerIndex\": " + String(playerIndex) +
                             ", \"point\": " + String(point) +
                             ", \"score\": " + String(scores[playerIndex]) + "}";
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

// Réinitialisation des points et scores pour une nouvelle partie
void resetGameState() {
  for (int i = 0; i < 4; i++) {
    points[i] = 0;
    scores[i] = 0;
  }
  gameEnded = false;
  Serial.println("🔄 État du jeu réinitialisé.");
}