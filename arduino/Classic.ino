
void EcranInitialisation() {
  lcd.print("Nb joueurs : " + String(nbJoueurs));
}

void InitGame() {
  FastLED.clear(); FastLED.show();
  myDFPlayer.stop();
  myDFPlayer.playMp3Folder(19);
  tourEnCours = 1;
  joueurEnCours = 1;
  resteEnCours = 3; // <<<--- Nb de tir
  tempstotalF = 5;
  dernierCountdownEnvoye = tempstotalF;
  TTRST();
  statusBoutonE = digitalRead(boutonE);
  if (nbJoueurs == 1) {
    nbJoueurs = 1;
  }
  if (nbJoueurs != oldNbJoueurs) {
    oldNbJoueurs = nbJoueurs;
  }
  initialisation = false;
  partieEnCours = true;
  EcranEnJeu();
  EcranGo();
}

void trtPartieEnCours() {
  FastLED.clear(); FastLED.show();
  Temporisation();
  EcranEnJeu();
  while (statusCible1 == HIGH && statusCible2 == HIGH && statusCible3 == HIGH && statusCible4 == HIGH && statusCible5 == HIGH && statusCible6 == HIGH && statusCible7 == HIGH && statusCible8 == HIGH && statusCible9 == HIGH && statusCible10 == HIGH) {
    AcquisitionCapteurs();
    Penalite();
    if (partieFinie == true) {
      break;
    } else {
      killer[joueurEnCours] = 0;
      }
    delay(1);
    }
    statusCible1 = digitalRead(cible1);
    if (statusCible1 == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show();
      myDFPlayer.playMp3Folder(1); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours] + 0 + pointBonus0[joueurEnCours];
      pointBonus0[joueurEnCours] = pointBonus0[joueurEnCours] + 0;
      resetAllBonusExcept(0);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(0) + " : " + String((pointBonus0[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
      // --- AJOUT pour gestion chrono spécial cible 1 ---
      // Met le chrono à zéro et envoie END_MANCHE immédiat
      dernierCountdownEnvoye = 0.0;
      float tempsUtilise = tempstotalF; // tout le temps a été utilisé
      Serial1.print("COUNTDOWN:");
      Serial1.println(0.000, 3);
      Serial1.print("END_MANCHE:");
      Serial1.print(joueurEnCours - 1);
      Serial1.print(":");
      Serial1.println(tempsUtilise, 3);
      // Ajoute 5 secondes à la manche en cours
      tempstotalF += 5.0;
      // Redémarre le chrono avec le nouveau temps total pour la manche
      startTime = millis();
      dernierCountdownEnvoye = tempstotalF;
    }
    statusCible2 = digitalRead(cible2);
    if (statusCible2 == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Orange); FastLED.show();
      myDFPlayer.playMp3Folder(2); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours] + 5 + pointBonus5[joueurEnCours];
      pointBonus5[joueurEnCours] = pointBonus5[joueurEnCours] + 1;
      resetAllBonusExcept(5);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(5) + " : " + String((pointBonus5[joueurEnCours]) -1)+ " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32 : " + message);
    }
    statusCible3 = digitalRead(cible3);
    if (statusCible3 == LOW) {
      coursesCommencees = false;
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Yellow); FastLED.show();
      myDFPlayer.playMp3Folder(3); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours] + 10 + pointBonus10[joueurEnCours];
      pointBonus10[joueurEnCours] = pointBonus10[joueurEnCours] + 2;
      resetAllBonusExcept(10);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(10) + " : " + String((pointBonus10[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
    statusCible4 = digitalRead(cible4);
    if (statusCible4 == LOW){
      coursesCommencees = false; 
     coursesFinies = false;                              
      fill_solid( leds, NUM_LEDS, CRGB::Green);  FastLED.show();
      myDFPlayer.playMp3Folder(5); delay(3000);
      FastLED.clear (); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours]+15+pointBonus15[joueurEnCours];
      pointBonus15[joueurEnCours] = pointBonus15[joueurEnCours]+3;
      resetAllBonusExcept(15);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(15) + " : " + String((pointBonus15[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }  
    statusCible5 = digitalRead(cible5);
    if (statusCible5 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      fill_solid(leds, NUM_LEDS, CRGB::Blue);  FastLED.show();
      myDFPlayer.playMp3Folder(8); delay(3000);
      FastLED.clear(); FastLED.show();
      scores[joueurEnCours] = scores[joueurEnCours]+25+pointBonus25[joueurEnCours];
      pointBonus25[joueurEnCours] = pointBonus25[joueurEnCours]+4;
      resetAllBonusExcept(25);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(25) + " : " + String((pointBonus25[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message); 
    }
    statusCible6 = digitalRead(cible6);
    if (statusCible6 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      Cinquante();               
      scores[joueurEnCours] = scores[joueurEnCours]+50+pointBonus50[joueurEnCours];
      pointBonus50[joueurEnCours] = pointBonus50[joueurEnCours]+5;
      resetAllBonusExcept(50);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(50) + " : " + String((pointBonus50[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
    statusCible7 = digitalRead(cible7);
    if (statusCible7 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      Cent();
      scores[joueurEnCours] = scores[joueurEnCours]+100+pointBonus100[joueurEnCours];
      pointBonus100[joueurEnCours] = pointBonus100[joueurEnCours]+10;
      resetAllBonusExcept(100);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(100) + " : " + String((pointBonus100[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
    statusCible8 = digitalRead(cible8);
    if (statusCible8 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      CentCinquante();
      scores[joueurEnCours] = scores[joueurEnCours]+150+pointBonus150[joueurEnCours];
      pointBonus150[joueurEnCours] = pointBonus150[joueurEnCours]+15;
      resetAllBonusExcept(150);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(150) + " : " + String((pointBonus150[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
    statusCible9 = digitalRead(cible9);
    if (statusCible9 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      DeuxCent();
      scores[joueurEnCours] = scores[joueurEnCours]+200+pointBonus200[joueurEnCours];
      pointBonus200[joueurEnCours] = pointBonus200[joueurEnCours]+20;
      resetAllBonusExcept(200);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(200) + " : " + String((pointBonus200[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
    statusCible10 = digitalRead(cible10);
    if (statusCible10 == LOW){
      coursesCommencees = false; 
      coursesFinies = false;
      DeuxCentCinquante();
      scores[joueurEnCours] = scores[joueurEnCours]+250+pointBonus250[joueurEnCours];
      pointBonus250[joueurEnCours] = pointBonus250[joueurEnCours]+25;
      resetAllBonusExcept(250);
      // Envoi du message structuré pour le joueur
      String message = "J" + String(joueurEnCours -1) + " : " + String(250) + " : " + String((pointBonus250[joueurEnCours]) -1) + " : " + String(scores[joueurEnCours]);
      Serial1.println(message);
      Serial.println("📤 Envoi à ESP32: " + message);
    }
  float tempsUtilise = tempstotalF - dernierCountdownEnvoye;
  Serial1.print("END_MANCHE:");
  Serial1.print(joueurEnCours - 1);
  Serial1.print(":");
  Serial1.println(tempsUtilise, 3);
  SMRAZ();
  GererInterruption();
}

void trtPartieFinie() {
  FinGame();
  Serial1.println("FIN_GAME");
  Serial.println("📤 Envoi à ESP32: FIN_GAME");
  while (killer[1] == 1 || killer[2] == 1 || killer[3] == 1 || killer[4] == 1) {
    Serial.println("KILLER STATUS J1 " + String(killer[1]) + String(killer[2]) + String(killer[3]) + String(killer[4]));
    for (int i = 1; i <= nbJoueurs; i++) {
      if (killer[i] == 1) {
        joueurEnCours = i;
      }
    }
  }
  partieFinie = false;
  partieEnCours = false;
  initialisation = true;
  oldNbJoueurs = 0;
  triclassement();
  AfficheFin();
}
