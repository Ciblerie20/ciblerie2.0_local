/*
ESP32 - WebSocket avec simulation et adaptation via RX/TX
Ce code simule la réception des données (pseudo et score) depuis un Arduino Mega via RX/TX,
ajoutant la génération et l'envoi directement dans l'ESP32.
*/

#include <WiFi.h>
#include <WebSocketsServer.h>

// Configuration WiFi
const char* ssid = "Livebox-9410";
const char* password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Configuration du port série pour la communication avec l'Arduino Mega
HardwareSerial SerialMega(1); // UART1 pour RX/TX (GPIO 16 et 17)

// Prototypes des fonctions
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void simulateRxTxData();

void setup() {
  // Initialisation des ports série
  Serial.begin(115200);                       // Logs pour le PC
  SerialMega.begin(9600, SERIAL_8N1, 16, 17); // RX/TX pour l'Arduino Mega

  // Initialisation du générateur de nombres aléatoires
  randomSeed(analogRead(15));

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
  // Gestion du WebSocket
  webSocket.loop();

  // Simulation de réception des données via RX/TX
  simulateRxTxData();

  // Lecture des données de l'Arduino Mega
  if (SerialMega.available()) {
    String message = SerialMega.readStringUntil('\n'); // Lecture jusqu'à un saut de ligne
    Serial.println("📥 Données reçues via RX/TX : " + message);

    // Transmission des données via WebSocket
    webSocket.broadcastTXT(message);
  }

  delay(2000); // Pause pour limiter la fréquence de simulation
}

// Simulation de réception des données via RX/TX
void simulateRxTxData() {
  // Génération des données simulées
  String pseudo = "Sébastien"; // Pseudo fixe
  int score = random(5, 1000); // Score aléatoire entre 5 et 1000

  // Création d'un message au format JSON
  String message = "{\"pseudo\": \"" + pseudo + "\", \"score\": " + String(score) + "}";

  // Simulation : Envoi des données simulées via le port série (comme si elles venaient d'un Arduino Mega)
  SerialMega.println(message);
  Serial.println("📤 Données simulées envoyées via RX/TX : " + message);
}

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("📨 Message reçu via WebSocket : %s\n", payload);
  }
}