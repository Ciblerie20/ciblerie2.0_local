
void Penalite() {
  if (!coursesCommencees)
  {
    EcranEnJeu();
    myDFPlayer.playMp3Folder(19);
    lcd.setCursor(11,3);
    lcd.print("TR: ");
    coursesCommencees = true;
//    Serial.println("coursesCommencees");
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
      tempsGenere = true;  // ✅ Marque que le temps a été généré
      // ✅ Affichage immédiat du temps restant pour éviter qu'il ne soit manquant
      lcd.setCursor(15,3);
      lcd.print(tempstotalF);
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
     lcd.setCursor(15,3);
     tempsrestant = tempstotalF - ( millis() - startTime) / 1000;
     if(tempsrestant < 0) 
     {
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
      // Remise à zéro des autres bonus
      resetAllBonuses();
      SMRAZ();
      EcranEnJeu();
      GererInterruption();  // 🔄 Passe au joueur suivant immédiatement 
     }
     else lcd.print(tempsrestant);
    }         
}
