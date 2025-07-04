
void Penalite() {
  if (!coursesCommencees)
  {
    EcranEnJeu();
    myDFPlayer.playMp3Folder(19);
    coursesCommencees = true;
    // Réinitialisation des couleurs et états
    memset(colorIndexes, 0, sizeof(colorIndexes)); // Réinitialise tous les index de couleur
    memset(lastColorChanges, 0, sizeof(lastColorChanges));
    startTime = millis();
    tempsGenere = false;  // ✅ Réinitialise l'état pour la prochaine manche
  }
  if(!coursesFinies && coursesCommencees) {
    if (!tempsGenere) {
      tempstotalF = 5;  // ⏳ Temps 5 secondes
      Period1 = tempstotalF * 1000;
      Period2 = tempstotalF * 1000;
      dernierCountdownEnvoye = tempstotalF;
      tempsGenere = true;  // ✅ Marque que le temps a été généré
      // 🎨 Affichage du code couleur correspondant
      if (tempstotalF == 1) fill_solid(leds, NUM_LEDS, CRGB::Red); // 🔴 Rouge
      else if (tempstotalF >= 2 && tempstotalF <= 5) { 
        int index = tempstotalF -2;
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]); // 🟠 Orange <- 🟡 Jaune <- 🟢 Vert <- 🔵 Bleu ( Sens de décomptage )
        lastColorChanges[index] = millis();
      }
      FastLED.show();
      Moteur();  // ✅ Activation immédiate
      // ✅ Démarre le délai non bloquant pour éteindre les LEDs après 1 seconde
      ledsAllumeesMillis = millis();
      ledsAllumees = true;
    }
     // ✅ Éteindre les LEDs après 1 seconde sans bloquer le code
     if (ledsAllumees && millis() - ledsAllumeesMillis >= 900) { //500) {
      FastLED.clear(); FastLED.show(); // ⭕ Éteint les LEDs après 1 seconde
      ledsAllumees = false;
     }
    // ✅ Changement de couleur toutes les secondes si tempstotalF entre 2 et 5
    if (tempstotalF >= 2 && tempstotalF <= 5) {
      int index = tempstotalF - 2;
      if (millis() - lastColorChanges[index] >= 1000) {
        colorIndexes[index] = (colorIndexes[index] + 1) % colorSizes[index];
        fill_solid(leds, NUM_LEDS, colorArrays[index][colorIndexes[index]]); //  🔵 Bleu -> 🟢 Vert -> 🟡 Jaune -> 🟠 Orange ( Sens réel )
        FastLED.show();
        lastColorChanges[index] = millis();
      }
    }
    // ⏳ Temps écoulé, on passe au joueur/tour suivant
    float tempsrestant = tempstotalF - (millis() - startTime) / 1000.0;
    lcd.setCursor(11, 3);
    lcd.print("TR: ");
    if (tempsrestant > 0) {
      lcd.print(tempsrestant, 3);
      Serial1.print("COUNTDOWN:");
      Serial1.println(tempsrestant, 3);
      dernierCountdownEnvoye = tempsrestant;
      zeroEnvoye = false;
    }
    else if (!coursesFinies && tempsrestant <= 0) {
      lcd.print("0.000");
      Serial1.print("COUNTDOWN:");
      Serial1.println(0.000, 3);
      dernierCountdownEnvoye = 0.0;
      zeroEnvoye = true;
    }
    if(tempsrestant < 0) 
    {
      float tempsUtilise = tempstotalF - dernierCountdownEnvoye;
      Serial1.print("END_MANCHE:");
      Serial1.print(joueurEnCours - 1);
      Serial1.print(":");
      Serial1.println(tempsUtilise, 3);
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
      resetAllBonuses();
      SMRAZ();
      EcranEnJeu();
      GererInterruption();
    }
  }
  else if (tempsrestant > 0) lcd.print(tempsrestant);
  else lcd.print("0.000");
}

void GererInterruption() {
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
    resteEnCours = 12; // <<<--- Nb de tir
    tourEnCours++;
    EcranTourSuivant();
    EcranJoueurSuivant();
    // === ENVOI "NEXT_TURN" ===
    Serial1.println("NEXT_TURN");
    Serial.println("📤 Envoi à ESP32: NEXT_TURN");
    Serial1.println(0.000, 3);
    lcd.clear();
    attenteJoueurOuTour = true;
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
      disableLedAnimation = false;
      disableSonAnimation = false;
      CodeLedAleatoire();
      SonsAleatoires();
      communications();
      delay(1);
    }
    boutonVirtuelE = false;
    attenteJoueurOuTour = false;
    EcranGo();               
  }
  else if (resteEnCours != 1) {
    resteEnCours--;
  }
  else {
    testKiller();
    joueurEnCours++;
    resteEnCours = 12; // <<<--- Nb de tir
    EcranJoueurSuivant(); 
    // === ENVOI "NEXT_PLAYER" ===
    Serial1.println("NEXT_PLAYER");
    Serial.println("📤 Envoi à ESP32: NEXT_PLAYER");
    Serial1.println(0.000, 3);
    lcd.clear();
    attenteJoueurOuTour = true;
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
      disableLedAnimation = false;
      disableSonAnimation = false;
      CodeLedAleatoire();
      SonsAleatoires();
      communications();
      delay(1);
    }
    boutonVirtuelE = false;
    attenteJoueurOuTour = false;
    EcranGo();
  }
}
