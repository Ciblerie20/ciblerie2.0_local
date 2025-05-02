/*
Arduino Mega - Simulation d'envoi de données via RX/TX
Envoie un pseudo fixe et un score aléatoire à l'ESP32.
*/

// Configuration du port série pour la communication avec l'ESP32
void setup() {
  Serial.begin(9600); // Communication avec l'ESP32
  randomSeed(analogRead(A0)); // Initialisation du générateur de nombres aléatoires
}

void loop() {
  // Génération des données
  String pseudo = "Sébastien"; // Pseudo fixe
  int score = random(5, 1000); // Score aléatoire entre 5 et 1000

  // Création d'un message au format JSON
  String message = "{\"pseudo\": \"" + pseudo + "\", \"score\": " + String(score) + "}";
  Serial.println(message); // Envoi des données à l'ESP32

  // Affichage pour vérification
  delay(2000); // Pause de 2 secondes avant le prochain envoi
}