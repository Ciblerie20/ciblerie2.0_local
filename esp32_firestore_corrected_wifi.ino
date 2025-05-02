
/*
esp32 - Firestore corrigé pour le wifi , code ok le 28.04.25 à 13H30
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h> // Pour obtenir l'heure via NTP

// Configuration WiFi
const char* ssid = "Livebox-9410"; // Remplacez par votre SSID WiFi
const char* password = "37322944"; // Remplacez par votre mot de passe WiFi

// Configuration Firestore API
const char* firestoreBaseUrl = "https://firestore.googleapis.com/v1/projects/ciblerie-2-0/databases/(default)/documents/scores"; // URL pour accéder à la collection `scores`

// Configuration du serveur NTP pour l'heure UTC
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;       // Décalage UTC (aucun décalage)
const int daylightOffset_sec = 0;  // Pas de décalage pour UTC

unsigned long lastWiFiCheck = 0;   // Dernière vérification de la connexion Wi-Fi
const unsigned long wifiCheckInterval = 10000; // Intervalle de vérification (en ms)
// Configuration du port série pour la communication avec l'Arduino Mega
HardwareSerial SerialMega(2);

void setup() {
  // Initialisation des ports série
  Serial.begin(115200);                       // Logs pour le PC
  SerialMega.begin(9600, SERIAL_8N1, 16, 17); // RX/TX pour communiquer avec l'Arduino Mega

  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());

  // Délai d'initialisation pour éviter les conflits après un téléversement
  delay(2000);

  connectToWiFi();

  // Configurer le serveur NTP pour UTC
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("⌛ Synchronisation de l'heure...");

  // Attendre que l'heure soit synchronisée
  while (time(nullptr) < 100000) {
    delay(100);
    Serial.print(".");
  }

  // Génération de valeurs aléatoires pour les tests
  randomSeed(analogRead(15));
  String testPseudo = "Sébastien"; // Pseudo
  int testScore = random(5, 1000); // Score aléatoire pour test
  sendScoreToFirestore(testPseudo, testScore);
}

void loop() {
  // Vérifier l'état du Wi-Fi régulièrement
  if (millis() - lastWiFiCheck >= wifiCheckInterval) {
    lastWiFiCheck = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("⚠️ Wi-Fi déconnecté. Tentative de reconnexion...");
      connectToWiFi();
    }
  }

  delay(1000); // Boucle simple pour tester
}

void connectToWiFi() {
  Serial.println("⌛ Connexion au WiFi...");

  // Réinitialisation complète du module Wi-Fi
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA); // S'assurer que le module est en mode station
  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis(); // Temps du début de la tentative
  const unsigned long wifiTimeout = 15000;  // Timeout pour la connexion Wi-Fi (15 secondes)

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < wifiTimeout) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Connecté au WiFi.");
    Serial.print("Adresse IP locale : ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\n❌ Impossible de se connecter au WiFi après plusieurs tentatives.");
    Serial.println("🔄 Redémarrage...");
    ESP.restart(); // Redémarrer l'ESP32 si la connexion échoue
  }
}

String getCurrentTimestampUTC() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("❌ Impossible d'obtenir l'heure UTC.");
    return "";
  }

  // Formatter l'heure en ISO 8601 UTC
  char buffer[30];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  return String(buffer);
}

void sendScoreToFirestore(String pseudo, int score) {
  HTTPClient http;
  http.begin(firestoreBaseUrl);
  http.addHeader("Content-Type", "application/json");

  // Obtenir le timestamp UTC actuel
  String timestamp = getCurrentTimestampUTC();

  if (timestamp == "") {
    Serial.println("❌ Impossible d'envoyer les données. Timestamp non valide.");
    return;
  }

  // Préparation des données JSON pour Firestore
  String jsonData = "{"
                      "\"fields\": {"
                        "\"pseudo\": {\"stringValue\": \"" + pseudo + "\"},"
                        "\"score\": {\"integerValue\": " + String(score) + "},"
                        "\"timestamp\": {\"stringValue\": \"" + timestamp + "\"}"
                      "}"
                    "}";

  Serial.println("📡 Envoi de score à Firestore...");
  Serial.println("📄 Données : " + jsonData);

  // Envoi de la requête POST
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.println("✅ Données envoyées à Firestore avec succès.");
    String response = http.getString();
    Serial.println("📨 Réponse Firestore : \"" + response + "\"");
  } else {
    Serial.println("❌ Erreur lors de l'envoi : " + String(httpResponseCode));
  }

  http.end();
}


