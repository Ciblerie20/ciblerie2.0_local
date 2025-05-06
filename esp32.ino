#include <WiFi.h>                // Bibliothèque WiFi pour ESP32
#include <WebSocketsServer.h>    // Serveur WebSocket
#include <BluetoothSerial.h>     // Bibliothèque Bluetooth

// Configuration WiFi (modifiable via Bluetooth)
String ssid = "Livebox-9410";
String password = "37322944";

// Création du serveur WebSocket sur le port 81
WebSocketsServer webSocket = WebSocketsServer(81);

// Création de l'objet Bluetooth Serial
BluetoothSerial SerialBT;

// Prototypes des fonctions
void connectToWiFi();
void ensureWiFiConnection(); // Vérifie et force la reconnexion au WiFi
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void processIncomingSerialData();
String convertToJSON(String data);
void handleBluetoothCommands(); // Gestion des commandes Bluetooth
void sendBluetoothWelcomeMessage(); // Message de bienvenue Bluetooth
void sendIPAddress(); // Nouvelle fonction pour envoyer l'adresse IP via Bluetooth

void setup() {
  Serial.begin(115200);

  // Initialisation de la liaison série avec l'Arduino Méga
  Serial1.begin(9600, SERIAL_8N1, 16, 17); // Utilisez directement Serial1
  Serial.println("🔗 Liaison série avec Arduino Méga initialisée.");

  // Initialisation Bluetooth
  SerialBT.begin("🖥️ ESP32_BT_Config"); // Nom Bluetooth de l'ESP32
  Serial.println("📡 Bluetooth initialisé. Connectez-vous à 'ESP32_BT_Config'.");
  
  // Envoyer un message de bienvenue et les commandes disponibles à la connexion
  SerialBT.register_callback([](esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    if (event == ESP_SPP_SRV_OPEN_EVT) {
      sendBluetoothWelcomeMessage();
    }
  });

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

  // Gestion des commandes Bluetooth
  handleBluetoothCommands();
}

// Connexion au réseau WiFi avec retour d'état
void connectToWiFi() {
  Serial.println("🌐 Connexion en cours...");
  SerialBT.println("🌐 Connexion en cours...");

  WiFi.begin(ssid.c_str(), password.c_str());

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) { // Essayer 20 fois avant d'abandonner
    delay(500);
    Serial.print(".");
    SerialBT.print(".");
    retries++;
  }

  Serial.println(""); // Saut de ligne après les points
  SerialBT.println("");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("✅ Connexion WiFi réussie !");
    SerialBT.println("✅ Connexion WiFi réussie !");
    sendIPAddress(); // Envoi l'adresse IP une fois connecté
  } else {
    Serial.println("❌ Connexion WiFi échouée. Vérifiez le SSID et le mot de passe.");
    SerialBT.println("❌ Connexion WiFi échouée. Vérifiez le SSID et le mot de passe.");
  }

  // Afficher les informations WiFi après tentative de connexion
  sendBluetoothWelcomeMessage();
}

// Vérifie et force la reconnexion au WiFi si nécessaire
void ensureWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi déconnecté. Tentative de reconnexion...");
    connectToWiFi();
  }
}

// Gestion des commandes Bluetooth
void handleBluetoothCommands() {
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();

    if (command.startsWith("SSID:")) {
      ssid = command.substring(5); // Extraire le SSID
      Serial.println("✅ SSID mis à jour : " + ssid);
      SerialBT.println("✅ SSID mis à jour : " + ssid);
      Serial.println("🌐 Connexion en cours...");
      SerialBT.println("🌐 Connexion en cours...");
      connectToWiFi(); // Reconnexion après changement
    } else if (command.startsWith("PASS:")) {
      password = command.substring(5); // Extraire le mot de passe
      Serial.println("✅ Mot de passe mis à jour.");
      SerialBT.println("✅ Mot de passe mis à jour.");
      Serial.println("🌐 Connexion en cours...");
      SerialBT.println("🌐 Connexion en cours...");
      connectToWiFi(); // Reconnexion après changement
    } else if (command == "RESTART") {
      Serial.println("🔄 Redémarrage de l'ESP32...");
      SerialBT.println("🔄 Redémarrage de l'ESP32...");
      delay(1000); // Petite pause avant redémarrage
      ESP.restart();
    } else {
      Serial.println("⚠️ Commande inconnue : " + command);
      SerialBT.println("⚠️ Commande inconnue : " + command);
    }
  }
}

// Message de bienvenue et rappel des commandes disponibles
void sendBluetoothWelcomeMessage() {
  SerialBT.println("🔗 Bienvenue sur ESP32_BT_Config !");
  SerialBT.println("Voici les informations actuelles et commandes disponibles :");

  // Informations sur l'état WiFi
  if (WiFi.status() == WL_CONNECTED) {
    SerialBT.println("✅ WiFi connecté !");
    sendIPAddress(); // Appeler la fonction d'envoi de l'adresse IP
  } else {
    SerialBT.println("❌ WiFi non connecté.");
  }

  // Afficher le SSID et le mot de passe actuels
  SerialBT.print("📡 SSID actuel : ");
  SerialBT.println(ssid);
  SerialBT.print("🔑 Mot de passe actuel : ");
  SerialBT.println(password);

  // Commandes disponibles
  SerialBT.println("\n⚙️ Commandes disponibles :");
  SerialBT.println(" - SSID:<NomDuReseau> : Changer le SSID.");
  SerialBT.println(" - PASS:<MotDePasse> : Changer le mot de passe.");
  SerialBT.println(" - RESTART : Redémarrer l'ESP32.");
}

// Fonction pour envoyer l'adresse IP via Bluetooth
void sendIPAddress() {
  if (WiFi.status() == WL_CONNECTED) {
    SerialBT.print("📶 Adresse IP : ");
    SerialBT.println(WiFi.localIP());
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

      if (incomingData.startsWith("J")) {
        int colon1 = incomingData.indexOf(':');
        int colon2 = incomingData.lastIndexOf(':');

        String joueur = incomingData.substring(0, colon1);
        String points = incomingData.substring(colon1 + 1, colon2);
        String cumul = incomingData.substring(colon2 + 1);

        Serial.printf("📥 Reçu : %s : %s : %s\n", joueur.c_str(), points.c_str(), cumul.c_str());

        String jsonData = "{\"playerIndex\": " + String(joueur.substring(1).toInt() - 1) +
                          ", \"point\": " + points +
                          ", \"score\": " + cumul + "}";

        webSocket.broadcastTXT(jsonData);
        Serial.printf("📤 Données envoyées : {\"playerIndex\": %d, \"point\": %s , \"score\": %s}\n",
                      joueur.substring(1).toInt() - 1, points.c_str(), cumul.c_str());

      } else if (incomingData.startsWith("Groupe : ")) {
        String groupe = incomingData.substring(9);
        Serial.printf("📥 Reçu : %s\n", groupe.c_str());
        String jsonData = "{\"type\": \"groupe\", \"value\": \"" + groupe + "\"}";
        webSocket.broadcastTXT(jsonData);
        Serial.println("📤 Données envoyées via WebSocket (JSON) : " + jsonData);

      } else if (incomingData == "FIN GAME") {
        Serial.println("📥 Reçu : FIN GAME");
        String jsonData = "{\"type\": \"fin\", \"message\": \"FIN GAME\"}";
        webSocket.broadcastTXT(jsonData);
        Serial.println("📤 Données envoyées via WebSocket (JSON) : " + jsonData);

      } else {
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