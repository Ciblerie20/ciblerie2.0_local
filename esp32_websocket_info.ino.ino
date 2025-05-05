
#include <WiFi.h>
#include <BluetoothSerial.h>

// Configuration WiFi (modifiable via Bluetooth)
String ssid = "Livebox-9410";
String password = "37322944";

// Création de l'objet Bluetooth Serial
BluetoothSerial SerialBT;

void connectToWiFi();
void ensureWiFiConnection();
void handleBluetoothCommands();
void sendBluetoothWelcomeMessage();

void setup() {
  Serial.begin(115200);

  // Initialisation Bluetooth
  SerialBT.begin("🖥️ ESP32_BT_Config"); // Nom Bluetooth de l'ESP32
  Serial.println("📡 Bluetooth initialisé. Connectez-vous à 'ESP32_BT_Config'.");
  
  // Envoyer un message de bienvenue et les commandes disponibles à la connexion
  SerialBT.register_callback([](esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
    if (event == ESP_SPP_SRV_OPEN_EVT) {
      sendBluetoothWelcomeMessage();
    }
  });

  // Connexion initiale au réseau WiFi
  connectToWiFi();
}

void loop() {
  // Vérifie et force la reconnexion WiFi si nécessaire
  ensureWiFiConnection();

  // Gestion des commandes Bluetooth
  handleBluetoothCommands();
}

// Connexion au réseau WiFi avec retour d'état
void connectToWiFi() {
  Serial.println("🌐 Tentative de connexion au WiFi...");
  SerialBT.println("🌐 Tentative de connexion au WiFi...");

  WiFi.begin(ssid.c_str(), password.c_str());

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) { // Essayer 20 fois avant d'abandonner
    delay(500);
    Serial.print(".");
    SerialBT.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connexion WiFi réussie !");
    SerialBT.println("\n✅ Connexion WiFi réussie !");
    Serial.print("📶 Adresse IP : ");
    Serial.println(WiFi.localIP());
    SerialBT.print("📶 Adresse IP : ");
    SerialBT.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Connexion WiFi échouée. Vérifiez le SSID et le mot de passe.");
    SerialBT.println("\n❌ Connexion WiFi échouée. Vérifiez le SSID et le mot de passe.");
  }
}

// Vérifie et force la reconnexion au WiFi si nécessaire
void ensureWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("⚠️ WiFi déconnecté. Tentative de reconnexion...");
    SerialBT.println("⚠️ WiFi déconnecté. Tentative de reconnexion...");
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
    } else if (command.startsWith("PASS:")) {
      password = command.substring(5); // Extraire le mot de passe
      Serial.println("✅ Mot de passe mis à jour.");
      SerialBT.println("✅ Mot de passe mis à jour.");
    } else if (command == "RESTART") {
      Serial.println("🔄 Redémarrage de l'ESP32...");
      SerialBT.println("🔄 Redémarrage de l'ESP32...");
      ESP.restart();
    } else {
      Serial.println("⚠️ Commande inconnue : " + command);
      SerialBT.println("⚠️ Commande inconnue : " + command);
    }

    // Toujours tenter de se reconnecter après un changement
    connectToWiFi();
  }
}

// Message de bienvenue et rappel des commandes disponibles
void sendBluetoothWelcomeMessage() {
  SerialBT.println("🔗 Bienvenue sur ESP32_BT_Config !");
  SerialBT.println("Voici les informations actuelles et commandes disponibles :");

  // Informations sur l'état WiFi
  if (WiFi.status() == WL_CONNECTED) {
    SerialBT.println("✅ WiFi connecté !");
    SerialBT.print("📶 Adresse IP : ");
    SerialBT.println(WiFi.localIP());
  } else {
    SerialBT.println("❌ WiFi non connecté.");
  }

  // Commandes disponibles
  SerialBT.println("\n⚙️ Commandes disponibles :");
  SerialBT.println(" - SSID:<NomDuReseau> : Changer le SSID.");
  SerialBT.println(" - PASS:<MotDePasse> : Changer le mot de passe.");
  SerialBT.println(" - RESTART : Redémarrer l'ESP32.");
}
