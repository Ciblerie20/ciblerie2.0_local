
// --------- CF ----------

void CF1() {
  Serial.println(F("CF1"));
  Serial.println(F("CF1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CF1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 1;
}

void CF2() {
  Serial.println(F("CF2"));
  Serial.println(F("CF2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CF2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 2;
}

void CF3() {
  Serial.println(F("CF3"));
  Serial.println(F("CF3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CF3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 3;
}

void CF4() {
  Serial.println(F("CF4"));
  Serial.println(F("CF4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CF4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 4;
}

// --------- CSP ----------

void CSP1() {
  Serial.println(F("CSP1"));
  Serial.println(F("CSP1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CSP1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 5;
}

void CSP2() {
  Serial.println(F("CSP2"));
  Serial.println(F("CSP2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CSP2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 6;
}

void CSP3() {
  Serial.println(F("CSP3"));
  Serial.println(F("CSP3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CSP3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 7;
}

void CSP4() {
  Serial.println(F("CSP4"));
  Serial.println(F("CSP4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CSP4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 8;
}

// --------- CP ----------

void CP1() {
  Serial.println(F("CP1"));
  Serial.println(F("CP1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CP1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 9;
}

void CP2() {
  Serial.println(F("CP2"));
  Serial.println(F("CP2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CP2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 10;
}

void CP3() {
  Serial.println(F("CP3"));
  Serial.println(F("CP3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CP3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 11;
}

void CP4() {
  Serial.println(F("CP4"));
  Serial.println(F("CP4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CP4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 12;
}

// --------- CC ----------

void CC1() {
  Serial.println(F("CC1"));
  Serial.println(F("CC1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CC1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 13;
}

void CC2() {
  Serial.println(F("CC2"));
  Serial.println(F("CC2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CC2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 14;
}

void CC3() {
  Serial.println(F("CC3"));
  Serial.println(F("CC3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CC3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 15;
}

void CC4() {
  Serial.println(F("CC4"));
  Serial.println(F("CC4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CC4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 16;
}

// --------- CL ----------

void CL1() {
  Serial.println(F("CL1"));
  Serial.println(F("CL1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CL1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 17;
}

void CL2() {
  Serial.println(F("CL2"));
  Serial.println(F("CL2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CL2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 18;
}

void CL3() {
  Serial.println(F("CL3"));
  Serial.println(F("CL3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CL3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 19;
}

void CL4() {
  Serial.println(F("CL4"));
  Serial.println(F("CL4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CL4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation == true) { InitGame(); }
  if (partieEnCours == true) { trtPartieEnCours(); }
  if (partieFinie == true) { trtPartieFinie(); }
  modeDeJeu = 20;
}

// --------- LJ ----------

void LJ1() {
  Serial.println(F("LJ1"));
  Serial.println(F("LJ1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de LJ1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameLJ();}   
  if (partieEnCours==true){trtPartieEnCoursLJ();} 
  if (partieFinie==true){trtPartieFinieLJ();}
  modeDeJeu = 21;
}

void LJ2() {
  Serial.println(F("LJ2"));
  Serial.println(F("LJ2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de LJ2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameLJ();}   
  if (partieEnCours==true){trtPartieEnCoursLJ();} 
  if (partieFinie==true){trtPartieFinieLJ();}
  modeDeJeu = 22;
}

void LJ3() {
  Serial.println(F("LJ3"));
  Serial.println(F("LJ3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de LJ3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameLJ();}   
  if (partieEnCours==true){trtPartieEnCoursLJ();} 
  if (partieFinie==true){trtPartieFinieLJ();}
  modeDeJeu = 23;
}

void LJ4() {
  Serial.println(F("LJ4"));
  Serial.println(F("LJ4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de LJ4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameLJ();}   
  if (partieEnCours==true){trtPartieEnCoursLJ();} 
  if (partieFinie==true){trtPartieFinieLJ();}
  modeDeJeu = 24;
}

// --------- MS ----------

void MS1() {
  Serial.println(F("MS1"));
  Serial.println(F("MS1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de MS1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameMS();}     
  if (partieEnCours==true){trtPartieEnCoursMS();} 
  if (partieFinie==true){trtPartieFinieMS();}
  modeDeJeu = 25;
}

void MS2() {
  Serial.println(F("MS2"));
  Serial.println(F("MS2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de MS2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameMS();}     
  if (partieEnCours==true){trtPartieEnCoursMS();} 
  if (partieFinie==true){trtPartieFinieMS();}
  modeDeJeu = 26;
}

void MS3() {
  Serial.println(F("MS3"));
  Serial.println(F("MS3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de MS3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameMS();}     
  if (partieEnCours==true){trtPartieEnCoursMS();} 
  if (partieFinie==true){trtPartieFinieMS();}
  modeDeJeu = 27;
}

void MS4() {
  Serial.println(F("MS4"));
  Serial.println(F("MS4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de MS4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameMS();}     
  if (partieEnCours==true){trtPartieEnCoursMS();} 
  if (partieFinie==true){trtPartieFinieMS();}
  modeDeJeu = 28;
}

// --------- CPT ----------

void CPT1() {
  Serial.println(F("CPT1"));
  Serial.println(F("CPT1, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CPT1 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs1;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameCPT();}   
  if (partieEnCours==true){trtPartieEnCoursCPT();} 
  if (partieFinie==true){trtPartieFinieCPT();}
  modeDeJeu = 29;
}

void CPT2() {
  Serial.println(F("CPT2"));
  Serial.println(F("CPT2, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CPT2 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs2;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameCPT();}   
  if (partieEnCours==true){trtPartieEnCoursCPT();} 
  if (partieFinie==true){trtPartieFinieCPT();}
  modeDeJeu = 30;
}

void CPT3() {
  Serial.println(F("CPT3"));
  Serial.println(F("CPT3, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CPT3 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs3;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameCPT();}   
  if (partieEnCours==true){trtPartieEnCoursCPT();} 
  if (partieFinie==true){trtPartieFinieCPT();}
  modeDeJeu = 31;
}

void CPT4() {
  Serial.println(F("CPT4"));
  Serial.println(F("CPT4, Ok pour le lancement du jeu"));
  Serial1.println("START_GAME");
  Serial.println("📨 Envoyer de CPT4 à ESP32: START_GAME");
  nbJoueurs = nbJoueurs4;
  AcquisitionCapteurs();
  if (initialisation==true){InitGameCPT();}   
  if (partieEnCours==true){trtPartieEnCoursCPT();} 
  if (partieFinie==true){trtPartieFinieCPT();}
  modeDeJeu = 32;
}
