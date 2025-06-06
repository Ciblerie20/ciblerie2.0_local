
// Mode Classique Pro (CP) mutualisé pour 1 à 4 joueurs

// Fonction principale du mode Pro
void jouerPartieCP(int nbJoueurs, String mode) {
  Serial.print("Mode : "); Serial.println(mode);
  Serial.println(F("CP , Ok pour le lancement du jeu"));
  envoyerDebutPartie();
  AcquisitionCapteurs();
  if (initialisation) InitGameCP(nbJoueurs);
  if (partieEnCours) trtPartieEnCoursCP(nbJoueurs);
  if (partieFinie) trtPartieFinieCP(nbJoueurs);
}

// Initialisation du jeu CP
void InitGameCP(int nbJoueurs) {
  FastLED.clear(); FastLED.show();
  myDFPlayer.stop();
  myDFPlayer.playMp3Folder(19);
  tourEnCours = 1;
  joueurEnCours = 1;
  resteEnCours = 3;
  for (int i = 1; i <= nbJoueurs; i++) {
    scores[i] = 0;
    killer[i] = 0;
    levels[i] = 0;
    oldClassement[i] = 0;
    classement[i] = 0;
  }
  statusBoutonE = digitalRead(boutonE);
  if (nbJoueurs == 1) nbJoueurs = 1;
  if (nbJoueurs != oldNbJoueurs) oldNbJoueurs = nbJoueurs;
  if (statusBoutonE == LOW) {
    for (int i = 1; i <= nbJoueurs; i++) killer[i] = 1;
  }
  initialisation = false;
  partieEnCours = true;
  EcranEnJeu();
  EcranGo();
}

// Boucle de partie CP
void trtPartieEnCoursCP(int nbJoueurs) {
  FastLED.clear(); FastLED.show();
  Temporisation();
  EcranEnJeu();
  while (
    statusCible1==HIGH && statusCible2==HIGH && statusCible3==HIGH && 
    statusCible4==HIGH && statusCible5==HIGH && statusCible6==HIGH && 
    statusCible7==HIGH && statusCible8==HIGH && statusCible9==HIGH && 
    statusCible10==HIGH
  ) {
    AcquisitionCapteurs();
    PenaliteCP(nbJoueurs);
    if (partieFinie) break;
    else killer[joueurEnCours] = 0;
    delay(1);
  }
  gestionCibles(joueurEnCours);
  SMRAZ();
  GererInterruptionCP(nbJoueurs);
}

// Pénalité CP : temps de réaction = 3 secondes
void PenaliteCP(int nbJoueurs) { 
  if (!coursesCommencees) {
    EcranEnJeu();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11,3);
    lcd.print("TR: ");
    coursesCommencees = true;
    memset(colorIndexes, 0, sizeof(colorIndexes));
    memset(lastColorChanges, 0, sizeof(lastColorChanges));
    startTime = millis();
    tempsGenere = false;
  }
  if (!coursesFinies && coursesCommencees) {
    if (!tempsGenere) {
      tempstotalCP = 3;
      Period1 = tempstotalCP * 1000;
      Period2 = tempstotalCP * 1000;
      tempsGenere = true;
      lcd.setCursor(15,3);
      lcd.print(tempstotalCP);
      if (tempstotalCP == 1) fill_solid(leds, NUM_LEDS, CRGB::Red);
      else if (tempstotalCP >= 2 && tempstotalCP <= 3) {
        int index = tempstotalCP - 2;
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]);
        lastColorChanges[index] = millis();
      }
      FastLED.show();
      Moteur();
      ledsAllumeesMillis = millis();
      ledsAllumees = true;
    }
    if (ledsAllumees && millis() - ledsAllumeesMillis >= 500) {
      FastLED.clear(); FastLED.show();
      ledsAllumees = false;
    }
    if (tempstotalCP >= 2 && tempstotalCP <= 3) {
      int index = tempstotalCP - 2;
      if (millis() - lastColorChanges[index] >= 1000) {
        colorIndexes[index] = (colorIndexes[index] + 1) % colorSizes[index];
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]);
        FastLED.show();
        lastColorChanges[index] = millis();
      }
    }
    lcd.setCursor(15,3);
    tempsrestant = tempstotalCP - ( millis() - startTime) / 1000;
    if (tempsrestant < 0) {
      coursesFinies = true;
      SMRAZTP();
      lcd.clear();
      clignotementLED(1);
      lcd.setCursor(3, 1);
      lcd.print("Temps ecoule !");
      clignotementLED(1);
      myDFPlayer.playMp3Folder(1);
      clignotementLED(1);
      Messages();
      clignotementLED(1);
      fill_solid(leds, NUM_LEDS, CRGB::Red);  FastLED.show();
      delay(2000);
      lcd.clear();
      FastLED.clear(); FastLED.show();
      coursesCommencees = false;
      coursesFinies = false;
      resetAllBonus0();
      SMRAZ();
      EcranEnJeu();
      GererInterruptionCP(nbJoueurs);
    } else lcd.print(tempsrestant);
  }
}

// Fin de partie CP
void trtPartieFinieCP(int nbJoueurs) {
  FinGame();
  while (killer[1]==1 || killer[2]==1 || killer[3]==1 || killer[4]==1 ) {
    Serial.println("KILLER STATUS J1 " + String(killer[1]) + String(killer[2]) + String(killer[3]) + String(killer[4]));   
    for (int i = 1; i <= nbJoueurs ; i++) {
      if (killer[i] == 1) joueurEnCours = i;
    } 
  }
  partieFinie = false;
  partieEnCours = false;
  initialisation = true;
  oldNbJoueurs = 0;
  triclassement();
  AfficheFin();
}

// Version unique de GererInterruption pour CP, basée sur CF1 
void GererInterruptionCP(int nbJoueurs) {
  if (joueurEnCours == nbJoueurs && tourEnCours == nbTours && resteEnCours == 1) {
    testKiller();
    coursesCommencees = false;
    coursesFinies = false;
    partieEnCours = false;
    partieFinie = true;
    SMRAZ();
  }
  else if (joueurEnCours == nbJoueurs && resteEnCours == 1) {
    myDFPlayer.stop();
    testKiller();
    joueurEnCours = 1;           
    resteEnCours = 3;
    tourEnCours++;
    EcranTourSuivant();    
    myDFPlayer.playMp3Folder(30);   
    EcranJoueurSuivant();  
    Serial1.println("NEXT_TURN");
    Serial.println("📤 Envoi à ESP32: NEXT_TURN");
    lcd.clear();
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
    boutonVirtuelE = false;
    EcranGo();
  }
  else if (resteEnCours != 1) {
    resteEnCours--; 
  }
  else {
    testKiller();
    joueurEnCours++;
    resteEnCours = 3;
    EcranJoueurSuivant();   
    myDFPlayer.playMp3Folder(30);
    myDFPlayer.playMp3Folder(23);   
    lcd.clear();
    Serial1.println("NEXT_PLAYER");
    Serial.println("📤 Envoi à ESP32: NEXT_PLAYER");
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
    boutonVirtuelE = false;
    EcranGo();
  }
  lcd.clear();
  // Pas de relance automatique de jouerPartieCP ici (menu/communications décide)
}

