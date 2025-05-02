#include <WiFi.h>
#include <WebSocketsServer.h>

// Configuration WiFi
const char* ssid = "Livebox-9410";
const char* password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Variables globales
unsigned long sendDelay = 3000;
unsigned long lastSendTime = 0;
int playerScores[4] = {0, 0, 0, 0};
int pointValues[] = {5, 10, 15, 25, 50};
int currentPlayer = 0;
int sendCount = 0;
bool gameEnded = false;

void connectToWiFi();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void updateScores();
void sendPlayerData();
void endGame();
void resetGameState();

void setup() {
  Serial.begin(115200);

  // Connexion au réseau WiFi
  connectToWiFi();

  // Démarrage du serveur WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  // Gestion des envois pendant une partie en cours
  if (!gameEnded && millis() - lastSendTime >= sendDelay) {
    lastSendTime = millis();
    sendPlayerData();
  }
}

// Connexion WiFi avec gestion de la reconnexion automatique
void connectToWiFi() {
  Serial.println("🌐 Connexion au WiFi...");
  WiFi.begin(ssid, password);

  int retryCount = 0;
  const int maxRetries = 10;

  while (WiFi.status() != WL_CONNECTED && retryCount < maxRetries) {
    delay(1000);
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

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    Serial.println("📨 Message reçu : " + message);

    if (message == "{\"type\":\"reset\"}") {
      resetGameState(); // Remise à zéro des scores et redémarrage
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

// Mise à jour du score du joueur actif avec attribution correcte des points
void updateScores() {
  int randomIndex = random(0, 5);
  int pointsWon = pointValues[randomIndex];
  playerScores[currentPlayer] += pointsWon;

  Serial.printf("🔢 Joueur %d gagne %d points (Nouveau score : %d)\n", 
                currentPlayer, pointsWon, playerScores[currentPlayer]);
}

// Envoi des données du joueur actuel
void sendPlayerData() {
  if (sendCount < 12) {
    updateScores();
    String jsonMessage = "{\"playerIndex\": " + String(currentPlayer) +
                         ", \"score\": " + String(playerScores[currentPlayer]) +
                         ", \"point\": " + String(pointValues[random(0, 5)]) + "}";
    webSocket.broadcastTXT(jsonMessage);
    Serial.println("📤 Données envoyées : " + jsonMessage);
    sendCount++;
  } else {
    currentPlayer++;
    sendCount = 0;
    if (currentPlayer >= 4) {
      endGame();
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

// Réinitialisation des scores et paramètres pour une nouvelle partie
void resetGameState() {
  currentPlayer = 0;
  sendCount = 0;
  gameEnded = false;
  for (int i = 0; i < 4; i++) {
    playerScores[i] = 0;
  }
  Serial.println("🔄 État du jeu réinitialisé.");
}
