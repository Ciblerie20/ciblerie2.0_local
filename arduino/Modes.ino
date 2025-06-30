
// ------------- MODES.ino (version sécurisée & factorisée) -------------

// Fonction utilitaire pour tous les modes classiques (CF, CSP, CP, CC, CL)
void lancerModeJeu(String nomMode, int nJoueurs, int codeMode) {
  if (partieDemarree) {
    Serial.println("⚠️ Tentative de relancer " + nomMode + " alors que la partie est déjà démarrée !");
    return;
  }
  Serial.println(nomMode);
  Serial.println(nomMode + ", Ok pour le lancement du jeu");
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de " + nomMode + " à ESP32: START_GAME");
  nbJoueurs = nJoueurs;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = codeMode;
  partieDemarree = true;
}

// --------- CF ----------
void CF1() { lancerModeJeu("CF1", nbJoueurs1, 1); }
void CF2() { lancerModeJeu("CF2", nbJoueurs2, 2); }
void CF3() { lancerModeJeu("CF3", nbJoueurs3, 3); }
void CF4() { lancerModeJeu("CF4", nbJoueurs4, 4); }

// --------- CSP ----------
void CSP1() { lancerModeJeu("CSP1", nbJoueurs1, 5); }
void CSP2() { lancerModeJeu("CSP2", nbJoueurs2, 6); }
void CSP3() { lancerModeJeu("CSP3", nbJoueurs3, 7); }
void CSP4() { lancerModeJeu("CSP4", nbJoueurs4, 8); }

// --------- CP ----------
void CP1() { lancerModeJeu("CP1", nbJoueurs1, 9); }
void CP2() { lancerModeJeu("CP2", nbJoueurs2, 10); }
void CP3() { lancerModeJeu("CP3", nbJoueurs3, 11); }
void CP4() { lancerModeJeu("CP4", nbJoueurs4, 12); }

// --------- CC ----------
void CC1() { lancerModeJeu("CC1", nbJoueurs1, 13); }
void CC2() { lancerModeJeu("CC2", nbJoueurs2, 14); }
void CC3() { lancerModeJeu("CC3", nbJoueurs3, 15); }
void CC4() { lancerModeJeu("CC4", nbJoueurs4, 16); }

// --------- CL ----------
void CL1() { lancerModeJeu("CL1", nbJoueurs1, 17); }
void CL2() { lancerModeJeu("CL2", nbJoueurs2, 18); }
void CL3() { lancerModeJeu("CL3", nbJoueurs3, 19); }
void CL4() { lancerModeJeu("CL4", nbJoueurs4, 20); }

// --------- LJ (Le Jeu) ----------
void LJ1() { LJMode("LJ1", nbJoueurs1, 21); }
void LJ2() { LJMode("LJ2", nbJoueurs2, 22); }
void LJ3() { LJMode("LJ3", nbJoueurs3, 23); }
void LJ4() { LJMode("LJ4", nbJoueurs4, 24); }
void LJMode(String nomMode, int nJoueurs, int codeMode) {
  if (partieDemarree) {
    Serial.println("⚠️ Tentative de relancer " + nomMode + " alors que la partie est déjà démarrée !");
    return;
  }
  Serial.println(nomMode);
  Serial.println(nomMode + ", Ok pour le lancement du jeu");
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de " + nomMode + " à ESP32: START_GAME");
  nbJoueurs = nJoueurs;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGameLJ(); }
  if (partieEnCours == true) { trtPartieEnCoursLJ(); }
  if (partieFinie == true) { trtPartieFinieLJ(); }
  modeDeJeu = codeMode;
  partieDemarree = true;
}

// --------- MS ----------
void MS1() { MSMode("MS1", nbJoueurs1, 25); }
void MS2() { MSMode("MS2", nbJoueurs2, 26); }
void MS3() { MSMode("MS3", nbJoueurs3, 27); }
void MS4() { MSMode("MS4", nbJoueurs4, 28); }
void MSMode(String nomMode, int nJoueurs, int codeMode) {
  if (partieDemarree) {
    Serial.println("⚠️ Tentative de relancer " + nomMode + " alors que la partie est déjà démarrée !");
    return;
  }
  Serial.println(nomMode);
  Serial.println(nomMode + ", Ok pour le lancement du jeu");
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de " + nomMode + " à ESP32: START_GAME");
  nbJoueurs = nJoueurs;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGameMS(); }
  if (partieEnCours == true) { trtPartieEnCoursMS(); }
  if (partieFinie == true) { trtPartieFinieMS(); }
  modeDeJeu = codeMode;
  partieDemarree = true;
}

// --------- CPT ----------
void CPT1() { CPTMode("CPT1", nbJoueurs1, 29); }
void CPT2() { CPTMode("CPT2", nbJoueurs2, 30); }
void CPT3() { CPTMode("CPT3", nbJoueurs3, 31); }
void CPT4() { CPTMode("CPT4", nbJoueurs4, 32); }
void CPTMode(String nomMode, int nJoueurs, int codeMode) {
  if (partieDemarree) {
    Serial.println("⚠️ Tentative de relancer " + nomMode + " alors que la partie est déjà démarrée !");
    return;
  }
  Serial.println(nomMode);
  Serial.println(nomMode + ", Ok pour le lancement du jeu");
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de " + nomMode + " à ESP32: START_GAME");
  nbJoueurs = nJoueurs;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGameCPT(); }
  if (partieEnCours == true) { trtPartieEnCoursCPT(); }
  if (partieFinie == true) { trtPartieFinieCPT(); }
  modeDeJeu = codeMode;
  partieDemarree = true;
}
