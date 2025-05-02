#include <WiFi.h>
#include <WebSocketsServer.h>

// Configuration WiFi
const char* ssid = "Livebox-9410"; // Remplacez par votre SSID WiFi
const char* password = "37322944"; // Remplacez par votre mot de passe WiFi

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Variables globales
unsigned long sendDelay = 3000; // Délai par défaut (3 secondes)
unsigned long lastSendTime = 0; // Temps du dernier envoi
int playerScores[4] = {0, 0, 0, 0}; // Scores initiaux pour 4 joueurs
int pointValues[] = {5, 10, 15, 25, 50}; // Points disponibles pour ajout aléatoire

// Prototypes des fonctions
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void updateScores();
void sendGameData();

void setup() {
  Serial.begin(115200);

  // Connexion au réseau WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connecté !");
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Démarrage du serveur WebSocket
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  webSocket.loop();

  // Envoi des données simulées toutes les X millisecondes
  if (millis() - lastSendTime >= sendDelay) {
    lastSendTime = millis();
    updateScores();
    sendGameData();
  }
}

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    Serial.println("📨 Message reçu : " + message);
  } else if (type == WStype_DISCONNECTED) {
    Serial.printf("❌ Client %d déconnecté\n", num);
  } else if (type == WStype_CONNECTED) {
    Serial.printf("✅ Client %d connecté\n", num);
  }
}

// Met à jour les scores des joueurs avec un ajout aléatoire
void updateScores() {
  for (int i = 0; i < 4; i++) {
    int randomIndex = random(0, 5); // Sélectionne un index aléatoire parmi les points disponibles
    playerScores[i] += pointValues[randomIndex]; // Ajoute des points au score du joueur
  }
}

// Envoi des scores des joueurs via WebSocket
void sendGameData() {
  for (int i = 0; i < 4; i++) {
    String jsonMessage = "{\"playerIndex\": " + String(i) + ", \"score\": " + String(playerScores[i]) + "}";
    webSocket.broadcastTXT(jsonMessage);
    Serial.println("📤 Données envoyées : " + jsonMessage);
  }
}