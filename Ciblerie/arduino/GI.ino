
void GererInterruption() {
  Serial.println("GererInterruption - Debut");
  Serial.print("joueurEnCours: "); Serial.println(joueurEnCours);
  Serial.print("tourEnCours: "); Serial.println(tourEnCours);
  Serial.print("resteEnCours: "); Serial.println(resteEnCours);
  Serial.print("nbJoueurs: "); Serial.println(nbJoueurs);
  Serial.print("nbTours: "); Serial.println(nbTours);
  if (joueurEnCours == nbJoueurs && tourEnCours == nbTours && resteEnCours == 1) {
    Serial.println("GererInterruption - Fin de partie");
    testKiller();
    coursesCommencees = false;
    coursesFinies = false;
    partieEnCours = false;
    partieFinie = true;
    SMRAZ();
  }
  else if (joueurEnCours == nbJoueurs && resteEnCours == 1) {
    Serial.println("GererInterruption - Nouveau tour");
    myDFPlayer.stop();
    testKiller();
    joueurEnCours = 1;           
    resteEnCours = 3;
    tourEnCours++;
    Serial.print("Nouveau tourEnCours: "); Serial.println(tourEnCours);
    EcranTourSuivant();    
    myDFPlayer.playMp3Folder(30);   
    EcranJoueurSuivant();
    // === ENVOI "NEXT_TURN" ===
    Serial1.println("NEXT_TURN");
    Serial.println("📤 Envoi à ESP32: NEXT_TURN");
    lcd.clear();
    // Attente du bouton physique OU virtuel
    while (digitalRead(boutonE) == HIGH && !boutonVirtuelE) {
      AcquisitionCapteurs();        
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0,0);
      lcd.print("Tour  : " + String(tourEnCours));
      lcd.setCursor(0,2);
      lcd.print("Quand pret");         
      lcd.setCursor(8,3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
      communications();
      delay(1);
    }
    boutonVirtuelE = false; // Reset du bouton virtuel après validation
    EcranGo();
    Serial.println("GO_TS");                  
  }
  else if (resteEnCours != 1) {
    Serial.println("GererInterruption - Reste en cours");
    resteEnCours--; 
  }
  else {
    Serial.println("GererInterruption - Nouveau joueur");
    testKiller();
    joueurEnCours++;
    resteEnCours = 3;
    EcranJoueurSuivant();   
    myDFPlayer.playMp3Folder(30);
    myDFPlayer.playMp3Folder(23);   
    lcd.clear();
    // === ENVOI "NEXT_PLAYER" ===
    Serial1.println("NEXT_PLAYER");
    Serial.println("📤 Envoi à ESP32: NEXT_PLAYER");

    // Attente du bouton physique OU virtuel
    while (digitalRead(boutonE) == HIGH && !boutonVirtuelE) {
      AcquisitionCapteurs();             
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0,0);
      lcd.print("Tour  : " + String(tourEnCours));
      lcd.setCursor(0,2);
      lcd.print("Quand pret");        
      lcd.setCursor(8,3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
      communications();
      delay(1);
    }
    boutonVirtuelE = false; // Reset du bouton virtuel après validation
    EcranGo();
    Serial.println("GO_JS");   
  }
  Serial.println("GererInterruption - Fin");
  // Ajout de l'appel à tous les groupes
  lcd.clear();
  // === Ici, appel dynamique du mode courant ===
  if (groupeSelectionne.startsWith("CF")) {
    if (groupeSelectionne == "CF1") { CF1(); }
    else if (groupeSelectionne == "CF2") { CF2(); }
    else if (groupeSelectionne == "CF3") { CF3(); }
    else if (groupeSelectionne == "CF4") { CF4(); }
  } 
  else if (groupeSelectionne.startsWith("CSP")) {
    if (groupeSelectionne == "CSP1") { CSP1(); }
    else if (groupeSelectionne == "CSP2") { CSP2(); }
    else if (groupeSelectionne == "CSP3") { CSP3(); }
    else if (groupeSelectionne == "CSP4") { CSP4(); }
  } 
  else if (groupeSelectionne.startsWith("CP")) {
    if (groupeSelectionne == "CP1") { CP1(); }
    else if (groupeSelectionne == "CP2") { CP2(); }
    else if (groupeSelectionne == "CP3") { CP3(); }
    else if (groupeSelectionne == "CP4") { CP4(); }
  } 
  else if (groupeSelectionne.startsWith("CC")) {
    if (groupeSelectionne == "CC1") { CC1(); }
    else if (groupeSelectionne == "CC2") { CC2(); }
    else if (groupeSelectionne == "CC3") { CC3(); }
    else if (groupeSelectionne == "CC4") { CC4(); }
  } 
  else if (groupeSelectionne.startsWith("CL")) {
    if (groupeSelectionne == "CL1") { CL1(); }
    else if (groupeSelectionne == "CL2") { CL2(); }
    else if (groupeSelectionne == "CL3") { CL3(); }
    else if (groupeSelectionne == "CL4") { CL4(); }
  } 
  else if (groupeSelectionne.startsWith("LJ")) {
    if (groupeSelectionne == "LJ1") { LJ1(); }
    else if (groupeSelectionne == "LJ2") { LJ2(); }
    else if (groupeSelectionne == "LJ3") { LJ3(); }
    else if (groupeSelectionne == "LJ4") { LJ4(); }
  } 
  else if (groupeSelectionne.startsWith("MS")) {
    if (groupeSelectionne == "MS1") { MS1(); }
    else if (groupeSelectionne == "MS2") { MS2(); }
    else if (groupeSelectionne == "MS3") { MS3(); }
    else if (groupeSelectionne == "MS4") { MS4(); }
  } 
  else if (groupeSelectionne == "CPT1") {
    CPT1(); 
  }
  else {
    Serial.println("Groupe non reconnu !");
  }
}
