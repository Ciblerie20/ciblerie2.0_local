#include <WiFi.h>                // Bibliothèque WiFi pour ESP32
#include <WebSocketsServer.h>    // Serveur WebSocket

// Configuration WiFi
const char* ssid = "Livebox-9410";
const char* password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

void connectToWiFi();
void ensureWiFiConnection(); // Vérifie et force la reconnexion au WiFi
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void processIncomingSerialData();
String convertToJSON(String data);

void setup() {
  Serial.begin(115200);

  // Initialisation de la liaison série avec l'Arduino Méga
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // Utilisez directement Serial1
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
  Serial.println("🔌 Tentative de connexion au WiFi...");

  WiFi.begin(ssid, password);

  // Attendre la connexion WiFi
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) { // Essayer 20 fois avant d'abandonner
    delay(500);
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi connecté !");
    Serial.print("📶 Adresse IP : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Impossible de se connecter au WiFi. Nouvelle tentative dans 5 secondes...");
    delay(5000); // Attendre 5 secondes avant une nouvelle tentative
  }
}

// Vérifie et force la reconnexion au WiFi si nécessaire
void ensureWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi déconnecté. Tentative de reconnexion...");
    connectToWiFi();
  }
}

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = String((char*)payload);
    Serial.println("📨 Message reçu : " + message);
  }
}

// Traitement des données série reçues de l'Arduino Méga
void processIncomingSerialData() {
  while (Serial1.available()) {
    String incomingData = Serial1.readStringUntil('\n'); // Lecture ligne par ligne
    incomingData.trim();

    if (!incomingData.isEmpty()) { // Vérifie que les données ne sont pas vides

      // Si les données correspondent à un joueur
      if (incomingData.startsWith("J")) {
        int colon1 = incomingData.indexOf(':');
        int colon2 = incomingData.lastIndexOf(':');

        // Extraction des informations
        String joueur = incomingData.substring(0, colon1);
        String points = incomingData.substring(colon1 + 1, colon2);
        String cumul = incomingData.substring(colon2 + 1);

        // Correction du format d'affichage
        Serial.printf("📥 Reçu : %s : %s : %s\n", joueur.c_str(), points.c_str(), cumul.c_str());

        // Conversion en JSON
        String jsonData = "{\"playerIndex\": " + String(joueur.substring(1).toInt() - 1) +
                          ", \"point\": " + points +
                          ", \"score\": " + cumul + "}";

        // Envoi via WebSocket
        webSocket.broadcastTXT(jsonData);
        Serial.printf("📤 Données envoyées : {\"playerIndex\": %d, \"point\": %s , \"score\": %s}\n",
                      joueur.substring(1).toInt() - 1, points.c_str(), cumul.c_str());

      } else if (incomingData.startsWith("Groupe : ")) {
        // Données correspondant à un groupe
        String groupe = incomingData.substring(9); // Extraire le nom du groupe
        Serial.printf("📥 Reçu : %s\n", groupe.c_str());
        String jsonData = "{\"type\": \"groupe\", \"value\": \"" + groupe + "\"}";
        webSocket.broadcastTXT(jsonData);
        Serial.println("📤 Données envoyées via WebSocket (JSON) : " + jsonData);

      } else if (incomingData == "FIN GAME") {
        // Données correspondant à la fin du jeu
        Serial.println("📥 Reçu : FIN GAME");
        String jsonData = "{\"type\": \"fin\", \"message\": \"FIN GAME\"}";
        webSocket.broadcastTXT(jsonData);
        Serial.println("📤 Données envoyées via WebSocket (JSON) : " + jsonData);

      } else {
        // Données générales ou autres formats
        String jsonData = convertToJSON(incomingData);
        webSocket.broadcastTXT(jsonData);
        Serial.println("📤 Données envoyées via WebSocket (JSON) : " + jsonData);
      }
    } else {
      Serial.println("⚠️ Données non valides reçues !");
    }
  }
}

// Fonction pour convertir les données reçues en JSON
String convertToJSON(String data) {
  String json = "{";

  if (data.startsWith("Groupe : ")) {
    json += "\"type\": \"groupe\", \"value\": \"" + data.substring(9) + "\"";
  } else if (data == "FIN GAME") {
    json += "\"type\": \"fin\", \"message\": \"FIN GAME\"";
  }

  json += "}";
  return json;
}