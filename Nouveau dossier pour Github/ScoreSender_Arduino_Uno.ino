#include <Arduino.h>

// Configuration des joueurs et scores
const int totalJoueurs = 4;             // Nombre de joueurs (J1 à J4)
const int totalScoresParJoueur = 12;    // Nombre de scores par joueur
const int intervalEnvoi = 2000;         // Intervalle entre chaque envoi (en ms)

// Valeurs possibles des scores
const int valeursScores[] = {5, 10, 15, 25, 50};
const int totalValeursScores = sizeof(valeursScores) / sizeof(valeursScores[0]);

// Variables pour la gestion des scores
int scoresJoueurs[totalJoueurs][totalScoresParJoueur]; // Table des scores pour chaque joueur
int joueurActuel = 0;                                  // Joueur en cours
int scoreActuelIndex = 0;                              // Score actuel à envoyer pour le joueur courant
unsigned long dernierEnvoi = 0;                        // Temps du dernier envoi
int scoreCumule = 0;                                   // Score cumulé pour le joueur courant

void setup() {
  // Initialisation des ports série
  Serial.begin(115200);    // Moniteur série pour le débogage

  // Génération des scores aléatoires pour chaque joueur
  randomSeed(analogRead(A0)); // Initialisation de la graine pour le générateur aléatoire
  for (int joueur = 0; joueur < totalJoueurs; joueur++) {
    for (int score = 0; score < totalScoresParJoueur; score++) {
      scoresJoueurs[joueur][score] = valeursScores[random(0, totalValeursScores)];
    }
  }

  Serial.println("🎯 Scores générés pour chaque joueur :");
  for (int joueur = 0; joueur < totalJoueurs; joueur++) {
    Serial.print("J" + String(joueur + 1) + ": ");
    for (int score = 0; score < totalScoresParJoueur; score++) {
      Serial.print(scoresJoueurs[joueur][score]);
      if (score < totalScoresParJoueur - 1) Serial.print(", ");
    }
    Serial.println();
  }
}

void loop() {
  // Gestion des envois toutes les 2 secondes
  if (millis() - dernierEnvoi >= intervalEnvoi) {
    dernierEnvoi = millis();

    // Mise à jour du score cumulé
    scoreCumule += scoresJoueurs[joueurActuel][scoreActuelIndex];

    // Envoi du score actuel et cumulé du joueur courant
    String message = "J" + String(joueurActuel + 1) + " : " + String(scoresJoueurs[joueurActuel][scoreActuelIndex]) + " : " + String(scoreCumule);
    Serial.println("📤 Envoi : " + message);

    // Passage au score suivant
    scoreActuelIndex++;
    if (scoreActuelIndex >= totalScoresParJoueur) {
      // Si tous les scores du joueur courant ont été envoyés, passer au joueur suivant
      scoreActuelIndex = 0;
      scoreCumule = 0; // Réinitialisation du score cumulé pour le joueur suivant
      joueurActuel++;

      // Si tous les joueurs ont été traités, terminer le jeu
      if (joueurActuel >= totalJoueurs) {
        Serial.println("📤 Envoi : FIN GAME");
        while (true); // Arrêt du programme après la fin du jeu
      }
    }
  }
}