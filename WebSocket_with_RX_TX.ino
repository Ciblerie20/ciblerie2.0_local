/*
ESP32 - WebSocket avec réception via RX/TX
Simule la réception des données (pseudo et score) depuis un Arduino Mega via RX/TX
et les transmet via WebSocket.
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

void setup() {
  // Initialisation des ports série
  Serial.begin(115200);                       // Logs pour le PC
  SerialMega.begin(9600, SERIAL_8N1, 16, 17); // RX/TX pour l'Arduino Mega

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

  // Lecture des données de l'Arduino Mega
  if (SerialMega.available()) {
    String message = SerialMega.readStringUntil('\n'); // Lecture jusqu'à un saut de ligne
    Serial.println("📥 Données reçues via RX/TX : " + message);

    // Transmission des données via WebSocket
    webSocket.broadcastTXT(message);
  }
}

// Gestion des événements WebSocket
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    Serial.printf("📨 Message reçu via WebSocket : %s\n", payload);
  }
}