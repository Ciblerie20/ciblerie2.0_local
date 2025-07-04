
// 📨 📥
void envoyerMessage(String message) {
  Serial1.println(message);
  Serial.print("📨 Message envoyé à ESP32 : ");
  Serial.println(message);
}

void communications() {
  if (Serial1.available() > 0) {
    String messageLu = Serial1.readStringUntil('\n');
    messageLu.trim();
    Serial.print("📥 Message reçu de l'ESP32 (brut) : ");
    Serial.println(messageLu); // Affiche le message brut
    char messageRecu[32]; // Tableau pour stocker le message reçu
    messageLu.toCharArray(messageRecu, 32);
    messageRecu[messageLu.length()] = '\0';
    Serial.print("📥 Message reçu de l'ESP32 : ");
    Serial.println(messageRecu);

    // 1. Bouton virtuel : SORTIE IMMÉDIATE
    if (strcmp(messageRecu, "VALID_E") == 0) {
      boutonVirtuelE = true;
      Serial.println("Validation bouton E reçue à distance !");
      return; // ←←← Ajout essentiel : on sort tout de suite de la fonction
    }

    // 2. Traitement d’un nom de groupe (valide uniquement) : IGNORER si partie déjà démarrée !
    if (messageRecu[0] != '\0'
        && (strcmp(messageRecu, "CF1") == 0 || strcmp(messageRecu, "CF2") == 0 || strcmp(messageRecu, "CF3") == 0 || strcmp(messageRecu, "CF4") == 0
            || strcmp(messageRecu, "CSP1") == 0 || strcmp(messageRecu, "CSP2") == 0 || strcmp(messageRecu, "CSP3") == 0 || strcmp(messageRecu, "CSP4") == 0
            || strcmp(messageRecu, "CP1") == 0 || strcmp(messageRecu, "CP2") == 0 || strcmp(messageRecu, "CP3") == 0 || strcmp(messageRecu, "CP4") == 0
            || strcmp(messageRecu, "CC1") == 0 || strcmp(messageRecu, "CC2") == 0 || strcmp(messageRecu, "CC3") == 0 || strcmp(messageRecu, "CC4") == 0
            || strcmp(messageRecu, "CL1") == 0 || strcmp(messageRecu, "CL2") == 0 || strcmp(messageRecu, "CL3") == 0 || strcmp(messageRecu, "CL4") == 0
            || strcmp(messageRecu, "LJ1") == 0 || strcmp(messageRecu, "LJ2") == 0 || strcmp(messageRecu, "LJ3") == 0 || strcmp(messageRecu, "LJ4") == 0
            || strcmp(messageRecu, "MS1") == 0 || strcmp(messageRecu, "MS2") == 0 || strcmp(messageRecu, "MS3") == 0 || strcmp(messageRecu, "MS4") == 0
            || strcmp(messageRecu, "CPT1") == 0)
        && !partieDemarree // <- AJOUT : on ignore si une partie est déjà démarrée
      ) {
      groupeSelectionne = String(messageRecu);
      Serial.print("Groupe sélectionné : ");
      Serial.println(groupeSelectionne);
      Serial.print("Groupe enregistré : ");
      Serial.println(groupeSelectionne);
      Serial1.println("START"); // Active le bouton "Attente groupe"
      Serial.print("📨 Message envoyé à ESP32 : ");
      Serial.println("START");
    }
    // Si message de groupe reçu en cours de partie, on le logue uniquement !
    else if (messageRecu[0] != '\0'
        && (strcmp(messageRecu, "CF1") == 0 || strcmp(messageRecu, "CF2") == 0 || strcmp(messageRecu, "CF3") == 0 || strcmp(messageRecu, "CF4") == 0
            || strcmp(messageRecu, "CSP1") == 0 || strcmp(messageRecu, "CSP2") == 0 || strcmp(messageRecu, "CSP3") == 0 || strcmp(messageRecu, "CSP4") == 0
            || strcmp(messageRecu, "CP1") == 0 || strcmp(messageRecu, "CP2") == 0 || strcmp(messageRecu, "CP3") == 0 || strcmp(messageRecu, "CP4") == 0
            || strcmp(messageRecu, "CC1") == 0 || strcmp(messageRecu, "CC2") == 0 || strcmp(messageRecu, "CC3") == 0 || strcmp(messageRecu, "CC4") == 0
            || strcmp(messageRecu, "CL1") == 0 || strcmp(messageRecu, "CL2") == 0 || strcmp(messageRecu, "CL3") == 0 || strcmp(messageRecu, "CL4") == 0
            || strcmp(messageRecu, "LJ1") == 0 || strcmp(messageRecu, "LJ2") == 0 || strcmp(messageRecu, "LJ3") == 0 || strcmp(messageRecu, "LJ4") == 0
            || strcmp(messageRecu, "MS1") == 0 || strcmp(messageRecu, "MS2") == 0 || strcmp(messageRecu, "MS3") == 0 || strcmp(messageRecu, "MS4") == 0
            || strcmp(messageRecu, "CPT1") == 0)
        && partieDemarree // <- Partie DÉJÀ démarrée !
      ) {
      Serial.print("⚠️ Message de groupe ignoré car partie déjà démarrée : ");
      Serial.println(messageRecu);
    }
    // 3. Démarrage de la partie par START_GAME (uniquement si pas déjà démarrée)
    else if (strcmp(messageRecu, "START_GAME") == 0 && !partieDemarree) {
      Serial.println("Message reçu : START_GAME");
      Serial.print("Partie démarrée avec le groupe : ");
      Serial.println(groupeSelectionne);
      if (groupeSelectionne == "CF1") { CF1(); }
      else if (groupeSelectionne == "CF2") { CF2(); }
      else if (groupeSelectionne == "CF3") { CF3(); }
      else if (groupeSelectionne == "CF4") { CF4(); }
      else if (groupeSelectionne == "CSP1") { CSP1(); }
      else if (groupeSelectionne == "CSP2") { CSP2(); }
      else if (groupeSelectionne == "CSP3") { CSP3(); }
      else if (groupeSelectionne == "CSP4") { CSP4(); }
      else if (groupeSelectionne == "CP1") { CP1(); }
      else if (groupeSelectionne == "CP2") { CP2(); }
      else if (groupeSelectionne == "CP3") { CP3(); }
      else if (groupeSelectionne == "CP4") { CP4(); }
      else if (groupeSelectionne == "CC1") { CC1(); }
      else if (groupeSelectionne == "CC2") { CC2(); }
      else if (groupeSelectionne == "CC3") { CC3(); }
      else if (groupeSelectionne == "CC4") { CC4(); }
      else if (groupeSelectionne == "CL1") { CL1(); }
      else if (groupeSelectionne == "CL2") { CL2(); }
      else if (groupeSelectionne == "CL3") { CL3(); }
      else if (groupeSelectionne == "CL4") { CL4(); }
      else if (groupeSelectionne == "LJ1") { LJ1(); }
      else if (groupeSelectionne == "LJ2") { LJ2(); }
      else if (groupeSelectionne == "LJ3") { LJ3(); }
      else if (groupeSelectionne == "LJ4") { LJ4(); }
      else if (groupeSelectionne == "MS1") { MS1(); }
      else if (groupeSelectionne == "MS2") { MS2(); }
      else if (groupeSelectionne == "MS3") { MS3(); }
      else if (groupeSelectionne == "MS4") { MS4(); }
      else if (groupeSelectionne == "CPT1") { CPT1(); }
      else {
        Serial.println("Groupe non reconnu !");
      }
      partieDemarree = true;
    }
    // 4. Gestion Joueur suivant et Tour suivant (identique)
    else if ((strcmp(messageRecu, "NEXT_PLAYER") == 0 || strcmp(messageRecu, "NEXT_TURN") == 0) && partieDemarree) {
      Serial.print("📥 Message reçu de l'ESP32 : ");
      Serial.println(messageRecu);
      Serial1.println("GO");
      Serial.print("📨 Message envoyé à ESP32 : ");
      Serial.println("GO");
    }
    // 5. Fin de partie
    else if (strcmp(messageRecu, "FIN_GAME") == 0 && partieDemarree) {
      Serial.println("📥 Message reçu de l'ESP32 : FIN_GAME");
      partieDemarree = false;
    }
  }
}
