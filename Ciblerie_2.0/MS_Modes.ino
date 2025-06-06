
// --- Mode MS Mutualisé (1 à 4 joueurs) ---

// Factorise MS1, MS2, MS3, MS4 en un seul bloc ultra-propre
void jouerPartieMS(int nbJoueurs, String mode) {
  Serial.print("Mode : "); Serial.println(mode);
  Serial.println(F("MS , Ok pour le lancement du jeu"));
  envoyerDebutPartie();
  AcquisitionCapteurs();
  if (initialisation) InitGameMS(nbJoueurs);
  if (partieEnCours) trtPartieEnCoursMS(nbJoueurs);
  if (partieFinie) trtPartieFinieMS(nbJoueurs);
}

// --- Initialisation du jeu ---
void InitGameMS(int nbJoueurs) {
  FastLED.clear(); FastLED.show();
  myDFPlayer.stop();
  myDFPlayer.playMp3Folder(19);
  tourEnCoursMS = 1;
  resteEnCoursMS = 1000000;
  joueurEnCours = 1;
  for (int i = 1; i <= 4; i++) {
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
  EcranEnJeuMS();
  EcranGo();
}

// --- Boucle principale de jeu ---
void trtPartieEnCoursMS(int nbJoueurs) {
  FastLED.clear(); FastLED.show();
  Temporisation();
  EcranEnJeuMS();

  while (
    statusCible1 == HIGH && statusCible2 == HIGH && statusCible3 == HIGH &&
    statusCible4 == HIGH && statusCible5 == HIGH && statusCible6 == HIGH &&
    statusCible7 == HIGH && statusCible8 == HIGH && statusCible9 == HIGH &&
    statusCible10 == HIGH
  ) {
    AcquisitionCapteurs();
    PenaliteMS(nbJoueurs);
    if (partieFinie) break;
    else killer[joueurEnCours] = 0;
    delay(1);
  }

  gestionCiblesMS(joueurEnCours);

  SMRAZ();
  GererInterruptionMS(nbJoueurs);
}

// --- Gestion factorisée des cibles pour MS (spécifique cible 1 !) ---
void gestionCiblesMS(int joueur) {
  // Cible 1 : mort subite générale
  statusCible1 = digitalRead(cible1);
  if (statusCible1 == LOW) {
    coursesCommencees = false;
    coursesFinies = false;
    fill_solid(leds, NUM_LEDS, CRGB::Red);  FastLED.show();
    myDFPlayer.playMp3Folder(1); delay(3000);
    FastLED.clear(); FastLED.show();
    resteEnCoursMS = 1;
    SMRAZ();
    GameOver();
    EcranEnJeuMS();
    partieFinie = true;
    return;
  }

  // Cibles 2 à 10 (logique CF)
  struct CibleData {
    int pin;
    CRGB couleur;
    int mp3Idx;
    int basePoints;
    int* pointBonus;
    void (*resetBonus)();
  } cibles[] = {
    {cible2, CRGB::Orange, 2, 5, &pointBonus5[0], resetAllBonus5},
    {cible3, CRGB::Yellow, 3, 10, &pointBonus10[0], resetAllBonus10},
    {cible4, CRGB::Green, 5, 15, &pointBonus15[0], resetAllBonus15},
    {cible5, CRGB::Blue, 8, 25, &pointBonus25[0], resetAllBonus25},
    {cible6, CRGB(0,255,255), 12, 50, &pointBonus50[0], resetAllBonus50},
    {cible7, CRGB(255,0,255), 14, 100, &pointBonus100[0], resetAllBonus100},
    {cible8, CRGB(255,255,0), 16, 150, &pointBonus150[0], resetAllBonus150},
    {cible9, CRGB::White, 18, 200, &pointBonus200[0], resetAllBonus200},
    {cible10, CRGB::Gray, 20, 250, &pointBonus250[0], resetAllBonus250}
  };
  int bonusIncr[] = {1,2,3,4,5,10,15,20,25};

  for (int i = 0; i < 9; i++) {
    if (digitalRead(cibles[i].pin) == LOW) {
      coursesCommencees = false; coursesFinies = false;
      fill_solid(leds, NUM_LEDS, cibles[i].couleur); FastLED.show();
      myDFPlayer.playMp3Folder(cibles[i].mp3Idx); delay(3000);
      FastLED.clear(); FastLED.show();
      int bonusVal = (*(cibles[i].pointBonus + joueur));
      scores[joueur] += cibles[i].basePoints + bonusVal;
      (*(cibles[i].pointBonus + joueur)) += bonusIncr[i];
      cibles[i].resetBonus();
      Serial.println(cibles[i].basePoints);

      // Message ESP32 - identique CF
      int basePoints = cibles[i].basePoints;
      int pointBonusValue = bonusVal;
      String message = "J" + String(joueur-1) + " : " + String(basePoints) + " : " + String(pointBonusValue) + " : " + String(scores[joueur]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
  }
}

// --- Pénalité MS (temps, mort subite générale en cas d'échec) ---
void PenaliteMS(int nbJoueurs) {
  if (!coursesCommencees) {
    EcranEnJeuMS();
    Moteur();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11, 3);
    lcd.print("TR: ");
    coursesCommencees = true;
    startTime = millis();
  }
  if (!coursesFinies && coursesCommencees) {
    lcd.setCursor(15, 3);
    tempsrestant = tempstotalL - (millis() - startTime) / 1000;
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
      resteEnCoursMS = 1;
      resetAllBonus0();
      SMRAZ();
      GameOver();
      EcranEnJeuMS();
      partieFinie = true;
    } else lcd.print(tempsrestant);
  }
}

// --- Fin de partie ---
void trtPartieFinieMS(int nbJoueurs) {
  FinGame();
  while (killer[1] == 1 || killer[2] == 1 || killer[3] == 1 || killer[4] == 1) {
    Serial.println("KILLER STATUS J1 " + String(killer[1]) + String(killer[2]) + String(killer[3]) + String(killer[4]));
    for (int i = 1; i <= nbJoueurs; i++) {
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

// --- Passage joueur/tour/fin ---
void GererInterruptionMS(int nbJoueurs) {
  if (joueurEnCours == nbJoueurs && tourEnCoursMS == nbToursMS && resteEnCoursMS == 1) {
    testKiller();
    coursesCommencees = false;
    coursesFinies = false;
    partieEnCours = false;
    partieFinie = true;
    SMRAZ();
  } else if (joueurEnCours == nbJoueurs && resteEnCoursMS == 1) {
    myDFPlayer.stop();
    testKiller();
    joueurEnCours = 1;
    resteEnCoursMS = 1000000;
    tourEnCoursMS++;
    EcranTourSuivantMS();
    myDFPlayer.playMp3Folder(30);
    EcranJoueurSuivant();
    lcd.clear();
    while (digitalRead(boutonE) == HIGH) {
      AcquisitionCapteurs();
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0, 0);
      lcd.print("Tour  : " + String(tourEnCoursMS));
      lcd.setCursor(0, 2);
      lcd.print("Quand pret");
      lcd.setCursor(8, 3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
    }
    EcranGo();
  } else if (resteEnCoursMS != 1) {
    resteEnCoursMS--;
  } else {
    myDFPlayer.stop();
    testKiller();
    joueurEnCours++;
    resteEnCoursMS = 1000000;
    myDFPlayer.playMp3Folder(30);
    EcranJoueurSuivant();
    lcd.clear();
    while (digitalRead(boutonE) == HIGH) {
      AcquisitionCapteurs();
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0, 0);
      lcd.print("Tour  : " + String(tourEnCoursMS));
      lcd.setCursor(0, 2);
      lcd.print("Quand pret");
      lcd.setCursor(8, 3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
    }
    EcranGo();
  }
  lcd.clear();
}

