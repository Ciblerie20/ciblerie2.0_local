
// --- Mode LJ Mutualisé (1 à 4 joueurs) ---

// À inclure dans ton projet, logique 100% factorisée et adaptée à LJ (mort subite, pénalité aléatoire, etc.)
void jouerPartieLJ(int nbJoueurs, String mode) {
  Serial.print("Mode : "); Serial.println(mode);
  Serial.println(F("LJ , Ok pour le lancement du jeu"));
  envoyerDebutPartie();
  AcquisitionCapteurs();
  if (initialisation) InitGameLJ(nbJoueurs);
  if (partieEnCours) trtPartieEnCoursLJ(nbJoueurs);
  if (partieFinie) trtPartieFinieLJ(nbJoueurs);
}

// --- Initialisation du jeu ---
void InitGameLJ(int nbJoueurs) {
  FastLED.clear(); FastLED.show();
  myDFPlayer.stop();
  myDFPlayer.playMp3Folder(19);
  tourEnCoursLJ = 1;
  resteEnCoursLJ = 1000000;
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
  EcranEnJeuLJ();
  EcranGo();
}

// --- Boucle principale de jeu ---
void trtPartieEnCoursLJ(int nbJoueurs) {
  FastLED.clear(); FastLED.show();
  Temporisation();
  EcranEnJeuLJ();

  while (
    statusCible1 == HIGH && statusCible2 == HIGH && statusCible3 == HIGH &&
    statusCible4 == HIGH && statusCible5 == HIGH && statusCible6 == HIGH &&
    statusCible7 == HIGH && statusCible8 == HIGH && statusCible9 == HIGH &&
    statusCible10 == HIGH
  ) {
    AcquisitionCapteurs();
    PenaliteLJ(nbJoueurs);
    if (partieFinie) break;
    else killer[joueurEnCours] = 0;
    delay(1);
  }

  gestionCiblesLJ(joueurEnCours);

  SMRAZ();
  GererInterruptionLJ(nbJoueurs);
}

// --- Gestion factorisée des cibles pour LJ (spécifique cible 1 !) ---
void gestionCiblesLJ(int joueur) {
  for (int i = 0; i < 10; ++i) {
    *(cibles[i].statusPtr) = digitalRead(cibles[i].pin);
    if (*(cibles[i].statusPtr) == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, cibles[i].couleur); FastLED.show();
      myDFPlayer.playMp3Folder(cibles[i].mp3FolderIdx); delay(3000);
      FastLED.clear(); FastLED.show();

      if (i == 0) { // Cible 1 = mort subite, passage immédiat, pas de points, pas de message ESP32
        resteEnCoursLJ = 1;
        SMRAZ();
        EcranEnJeuLJ();
        // PAS de gestion score/bonus/message
        break; // On sort, car on ne veut pas traiter les autres cibles
      } else {
        int bonusValBefore = cibles[i].pointBonusArray[joueur];
        scores[joueur] += cibles[i].basePoints + cibles[i].pointBonusArray[joueur];
        int bonusInc = 0;
        switch (i) {
          case 1: bonusInc = 1; break;
          case 2: bonusInc = 2; break;
          case 3: bonusInc = 3; break;
          case 4: bonusInc = 4; break;
          case 5: bonusInc = 5; break;
          case 6: bonusInc = 10; break;
          case 7: bonusInc = 15; break;
          case 8: bonusInc = 20; break;
          case 9: bonusInc = 25; break;
        }
        cibles[i].pointBonusArray[joueur] += bonusInc;
        if (cibles[i].bonusFn != nullptr) cibles[i].bonusFn();
        cibles[i].resetAllBonus();
        Serial.println(cibles[i].basePoints);

        // Message à l’ESP32
        int basePoints = cibles[i].basePoints;
        int pointBonusValue = bonusValBefore;
        String message = "J" + String(joueur-1) + " : " + String(basePoints) + " : " + String(pointBonusValue) + " : " + String(scores[joueur]);
        Serial1.println(message);
        Serial.println("📤 Envoi à ESP32: " + message);
      }
    }
  }
}

// --- Pénalité LJ (temps aléatoire, passage auto au joueur suivant) ---
void PenaliteLJ(int nbJoueurs) {
  if (!coursesCommencees) {
    EcranEnJeuLJ();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11, 3);
    lcd.print("TR: ");
    coursesCommencees = true;
    memset(colorIndexes, 0, sizeof(colorIndexes));
    memset(lastColorChanges, 0, sizeof(lastColorChanges));
    startTime = millis();
    tempsGenere = false;
  }
  if (!coursesFinies && coursesCommencees) {
    if (!tempsGenere) {
      TempsAleatoireLJ = random(1, 6); // 1 à 5 sec
      Period1LJ = TempsAleatoireLJ * 1000;
      Period2LJ = TempsAleatoireLJ * 1000;
      tempsGenere = true;
      lcd.setCursor(15, 3);
      lcd.print(TempsAleatoireLJ);
      if (TempsAleatoireLJ == 1) fill_solid(leds, NUM_LEDS, CRGB::Red);
      else if (TempsAleatoireLJ >= 2 && TempsAleatoireLJ <= 5) {
        int index = TempsAleatoireLJ - 2;
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
    if (TempsAleatoireLJ >= 2 && TempsAleatoireLJ <= 5) {
      int index = TempsAleatoireLJ - 2;
      if (millis() - lastColorChanges[index] >= 1000) {
        colorIndexes[index] = (colorIndexes[index] + 1) % colorSizes[index];
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]);
        FastLED.show();
        lastColorChanges[index] = millis();
      }
    }
    lcd.setCursor(15, 3);
    tempsrestant = TempsAleatoireLJ - (millis() - startTime) / 1000;
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
      resteEnCoursLJ = 1; // Mort subite : passage auto
      resetAllBonus0();
      SMRAZ();
      EcranEnJeuLJ();
      GererInterruptionLJ(nbJoueurs);
    } else lcd.print(tempsrestant);
  }
}

// --- Gestion de fin de partie ---
void trtPartieFinieLJ(int nbJoueurs) {
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

// --- Passage joueur/tour/fin, logique LJ (diff CF1 !) ---
void GererInterruptionLJ(int nbJoueurs) {
  if (joueurEnCours == nbJoueurs && tourEnCoursLJ == nbToursLJ && resteEnCoursLJ == 1) {
    testKiller();
    coursesCommencees = false;
    coursesFinies = false;
    partieEnCours = false;
    partieFinie = true;
    SMRAZ();
  } else if (joueurEnCours == nbJoueurs && resteEnCoursLJ == 1) {
    myDFPlayer.stop();
    testKiller();
    joueurEnCours = 1;
    resteEnCoursLJ = 1000000;
    tourEnCoursLJ++;
    EcranTourSuivantLJ();
    myDFPlayer.playMp3Folder(30);
    EcranJoueurSuivant();
    lcd.clear();
    while (digitalRead(boutonE) == HIGH) {
      AcquisitionCapteurs();
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0, 0);
      lcd.print("Tour  : " + String(tourEnCoursLJ));
      lcd.setCursor(0, 2);
      lcd.print("Quand pret");
      lcd.setCursor(8, 3);
      lcd.print("Start --> OK");
      FastLED.clear(); FastLED.show();
    }
    EcranGo();
  } else if (resteEnCoursLJ != 1) {
    resteEnCoursLJ--;
  } else {
    myDFPlayer.stop();
    testKiller();
    joueurEnCours++;
    resteEnCoursLJ = 1000000;
    EcranTourSuivantLJ();
    myDFPlayer.playMp3Folder(30);
    EcranJoueurSuivant();
    lcd.clear();
    while (digitalRead(boutonE) == HIGH) {
      AcquisitionCapteurs();
      printBigNum(21, 13, 0);
      printBigNum(joueurEnCours, 17, 0);
      lcd.setCursor(0, 0);
      lcd.print("Tour  : " + String(tourEnCoursLJ));
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

