
void SMRAZ(){ // Remise des servos à zéro
  EcranScores1();
    for (int i = 0; i < NUM_SERVOS; i++) {
        pwm.setPWM(i, 0, pulseWidth(0));
    }
    delay(500);
}

void SMRAZTP(){ // Action pour faire tomber la cible
  EcranScores1();
    // Position initiale (0)
    for (int i = 0; i < NUM_SERVOS; i++) {
        pwm.setPWM(i, 0, pulseWidth(0));
    }
    delay(500);
    // Position pour faire tomber la cible (85)
    for (int i = 0; i < NUM_SERVOS; i++) {
        pwm.setPWM(i, 0, pulseWidth(85));
    }
    delay(500);
}

// ✅ Nouvelle version de MoteurLJ() avec activation/désactivation propre des cibles (moteurs)
void Moteur() {
    randomSeed(analogRead(13));
    int Motor_aleatoire = random(0, 3); // Choisir un moteur aléatoire entre 0 et 2
    // Désactiver les moteurs si nécessaire
    if (millis() - Time1 >= Period1) {
        pwm.setPWM(Motor_aleatoire, 0, pulseWidth(0)); // Éteint le moteur sélectionné
        Time1 = millis();
    }
    // Activer un moteur aléatoire
    if (millis() - Time2 >= Period2) {
//        for (int i = 0; i < 16; i++) {
        for (int i = 0; i < NUM_SERVOS; i++) {
            pwm.setPWM(i, 0, pulseWidth(0)); // Éteindre tous les moteurs
        }
        pwm.setPWM(Motor_aleatoire, 0, pulseWidth(85)); // Activer le moteur sélectionné
        // AJOUT : ENVOYER LE NUMERO DU SERVOMOTEUR LEVÉ À L'ESP32
        Serial1.print("SERVO_UP:"); Serial1.println(Motor_aleatoire);
        // Tu peux aussi utiliser Serial1.printf("SERVO_UP:%d\n", Motor_aleatoire);
        Time2 = millis();
    }
}

void EcranTourSuivant(){
  EcranScores1();  
  lcd.clear();
  printBigNum(31, 2, 1);
  printBigNum(26, 5, 1);
  printBigNum(32, 8, 1);
  printBigNum(29, 11, 1); 
  printBigNum(tourEnCours,15,1);
  delay(2000);
  EcranScores1();
}

void EcranJoueurSuivant(){
  EcranScores1();
  lcd.clear();
  printBigNum(21, 6, 1);
  printBigNum(joueurEnCours,10,1);
  delay(2000);
  EcranScores1(); 
}

void EcranGo() {
  EcranScores1();
  myDFPlayer.playMp3Folder(31);
  delay(3000);
  EcranScores1();
  lcd.clear();
  printBigNum(18, 7, 1);
  printBigNum(26, 11, 1);
  delay(1000);  
}

void EcranGoJeux(){
  EcranScores1();
  lcd.clear();
  printBigNum(18, 7, 1);
  printBigNum(26, 11, 1);
  delay(1000);   
}

void Oups(){
  EcranScores1();
  lcd.clear();
  printBigNum(26, 3, 0); //O
  printBigNum(32, 7, 0); //U
  printBigNum(27, 11, 0); //P
  printBigNum(30, 15, 0); //S
  delay(2000);
}

void GameOver()
{
  EcranScores1();
  lcd.clear();  
  printBigNum(18, 3, 0); //G
  printBigNum(12, 7, 0); //A
  printBigNum(24, 11, 0); //M
  printBigNum(16, 15, 0); //E
  printBigNum(26, 3, 2); //O
  printBigNum(33, 7, 2); //V
  printBigNum(16, 11, 2); //E
  printBigNum(29, 15, 2); //R  
  delay(2000);
  EcranScores1();
}

void FinGame(){
  EcranScores1();
  lcd.clear();
  printBigNum(17, 3, 0); //F
  printBigNum(20, 7, 0); //I
  printBigNum(25, 11, 0); //N 
  printBigNum(18, 3, 2); //G
  printBigNum(12, 7, 2); //A
  printBigNum(24, 11, 2); //M
  printBigNum(16, 15, 2); //E
  EcranScores1();
  delay(2000);
  EcranScores1();
}

void clignotementLED(int repetitions){
  for (int i = 0; i < repetitions; i++) {
    fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show(); delay(50);
    fill_solid(leds, NUM_LEDS, CRGB::Orange); FastLED.show(); delay(50);
    fill_solid(leds, NUM_LEDS, CRGB::Yellow); FastLED.show(); delay(50);
  }
}

void Cinquante(){ //50
  EcranScores1();
  lcd.clear();
  myDFPlayer.stop();
  for (int i = 0; i < 3; i++) { // Répète 3 fois
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(5, 6, 1); printBigNum(0, 10, 1); delay(300); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::White); FastLED.show(); printBigNum(5, 6, 1); printBigNum(0, 10, 1); delay(300); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Red); FastLED.show(); printBigNum(5, 6, 1); printBigNum(0, 10, 1); delay(300); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Black); FastLED.show(); printBigNum(5, 6, 1); printBigNum(0, 10, 1); delay(300); FastLED.clear(); FastLED.show();
  // delay de 1s200 part cycle
  }
// Delay total 3s600 d'intéruption
}

void Cent(){ //100
  EcranScores1();
  lcd.clear();
  myDFPlayer.stop();
  for (int i = 0; i < 3; i++) { // Répète 3 fois
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(1, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(250); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(1, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(250); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(1, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(250); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(1, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(250); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Black); FastLED.show(); printBigNum(1, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(250); FastLED.clear(); FastLED.show();
  // delay de 1s250 part cycle
  }
  fill_solid(leds, NUM_LEDS, CRGB::Green);  FastLED.show(); printBigNum(1, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(500); FastLED.clear(); FastLED.show();
// Delay total 4s250 d'intéruption
}

void CentCinquante(){ //150
  EcranScores1();
  lcd.clear();
  myDFPlayer.stop();
  for (int i = 0; i < 4; i++) { // Répète 4 fois
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(200); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(200); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Magenta); FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(200); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(200); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(200); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Magenta); FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(200); FastLED.clear(); FastLED.show();
  // delay de 1s200 part cycle
  }
  fill_solid(leds, NUM_LEDS, CRGB::Orange);  FastLED.show(); printBigNum(1, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(500); FastLED.clear(); FastLED.show();
// Delay total 5s300 d'intéruption
}

void DeuxCent(){ //200
  EcranScores1();
  lcd.clear();
  myDFPlayer.stop();
  for (int i = 0; i < 4; i++) { // Répète 4 fois
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Magenta); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Cyan); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Magenta); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Cyan); FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  // delay de 1s200 part cycle
  }
  fill_solid(leds, NUM_LEDS, CRGB::Yellow);  FastLED.show(); printBigNum(2, 4, 1); printBigNum(0, 8, 1); printBigNum(0, 12, 1); delay(500); FastLED.clear(); FastLED.show();
// Delay total 5s300 d'intéruption
}

void DeuxCentCinquante(){ //250
  EcranScores1();
  lcd.clear();
  myDFPlayer.stop();
  for (int i = 0; i < 4; i++) { // Répète 4 fois
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Magenta); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Cyan); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Purple); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Green); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Blue); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Magenta); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Cyan); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  fill_solid(leds, NUM_LEDS, CRGB::Purple); FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(150); FastLED.clear(); FastLED.show();
  // delay de 1s500 part cycle
  }
  fill_solid(leds, NUM_LEDS, CRGB::White);  FastLED.show(); printBigNum(2, 4, 1); printBigNum(5, 8, 1); printBigNum(0, 12, 1); delay(500); FastLED.clear(); FastLED.show();
// Delay total 6s500 d'intéruption
}

void EcranEnJeu() {
  lcd1.clear();
  lcd1.setCursor(0,0);
  lcd1.print("J1 : " + String(scores[1]));
  Serial.println("J1 : " + String(scores[1]));
  lcd1.setCursor(0,1);
  lcd1.print("J2 : " + String(scores[2]));
  Serial.println("J2 : " + String(scores[2]));
  lcd1.setCursor(0,2);
  lcd1.print("J3 : " + String(scores[3]));
  Serial.println("J3 : " + String(scores[3]));
  lcd1.setCursor(0,3);
  lcd1.print("J4 : " + String(scores[4]));
  Serial.println("J4 : " + String(scores[4]));

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tour : "+ String(tourEnCours)+"  Reste : "+ String(resteEnCours));
  Serial.println("Tour : "+ String(tourEnCours)+" Reste : "+ String(resteEnCours));
  lcd.setCursor(0,3);
  lcd.print("Joueur: " + String(joueurEnCours));
  Serial.println("Joueur: " + String(joueurEnCours));
  
  String scorestring = String(scores[joueurEnCours]);
  // Code de 100.000 à 999.999  
    if (scorestring.length()==6){
      String Cmille  = String(scorestring.charAt(0));
      printBigNum(Cmille  .toInt(), 2, 1);      
      String Dmille  = String(scorestring.charAt(1));
      printBigNum(Dmille  .toInt(), 5, 1);      
      String millier = String(scorestring.charAt(2));
      printBigNum(millier.toInt(), 8, 1);      
      String centaine = String(scorestring.charAt(3));
      printBigNum(centaine.toInt(), 11, 1);
      String dizaine = String(scorestring.charAt(4));
      printBigNum(dizaine.toInt(), 14, 1);
      String unite = String(scorestring.charAt(5));
      printBigNum(unite.toInt(), 17, 1);
      }
  // Code de 10.000 à 99.999     
    else if (scorestring.length()==5){
      String Dmille  = String(scorestring.charAt(0));
      printBigNum(Dmille  .toInt(), 5, 1);      
      String millier = String(scorestring.charAt(1));
      printBigNum(millier.toInt(), 8, 1);      
      String centaine = String(scorestring.charAt(2));
      printBigNum(centaine.toInt(), 11, 1);
      String dizaine = String(scorestring.charAt(3));
      printBigNum(dizaine.toInt(), 14, 1);
      String unite = String(scorestring.charAt(4));
      printBigNum(unite.toInt(), 17, 1);
      }
  // Code de 1.000 à 9.999
    else if (scorestring.length()==4){
      String millier = String(scorestring.charAt(0));
      printBigNum(millier.toInt(), 8, 1);      
      String centaine = String(scorestring.charAt(1));
      printBigNum(centaine.toInt(), 11, 1);
      String dizaine = String(scorestring.charAt(2));
      printBigNum(dizaine.toInt(), 14, 1);
      String unite = String(scorestring.charAt(3));
      printBigNum(unite.toInt(), 17, 1);
    }
  // Code de 100 à 999      
    else if (scorestring.length()==3){
      String centaine = String(scorestring.charAt(0));
      printBigNum(centaine.toInt(), 11, 1);
      String dizaine = String(scorestring.charAt(1));
      printBigNum(dizaine.toInt(), 14, 1);
      String unite = String(scorestring.charAt(2));
      printBigNum(unite.toInt(), 17, 1);
    }
  // Code de 10 à 99    
    else if (scorestring.length()==2){
      String dizaine = String(scorestring.charAt(0));
      printBigNum(dizaine.toInt(), 14, 1); // 12
      String unite = String(scorestring.charAt(1));
      printBigNum(unite.toInt(), 17, 1); // 15
    }
  // Code de 01 à 09    
    else{
      String unite = String(scorestring.charAt(0));
      printBigNum(0, 14, 1); // 12
      printBigNum(unite.toInt(), 17, 1); // 15     
    }         
}

void EcranScores() {
    EcranScores1();
    lcd.clear();
    for (int i = 1; i <= 4; i++) {
        lcd.setCursor(0, i - 1);
        lcd.print("J" + String(i) + " : " + String(scores[i]));
        Serial.println("J" + String(i) + " : " + String(scores[i]));
    }
    delay(4000);
}

void EcranScores1() {
    if (millis() - Time18M >= Period18M) {
        lcd1.clear();
        for (int i = 1; i <= 4; i++) {
            lcd1.setCursor(0, i - 1);
            lcd1.print("J" + String(i) + " : " + String(scores[i]));
            Serial.println("J" + String(i) + " : " + String(scores[i]));
        }
        Time18M = millis();
    }
}

void testKiller(){
  EcranScores1();
  if (killer[joueurEnCours] == 1){
  }
}

void AcquisitionCapteurs() {
    // Tableau des capteurs et boutons (de type bool*)
    bool* capteurs[] = {&statusBoutonE, &statusBoutonG, &statusBoutonD, &statusCible1, &statusCible2, 
                        &statusCible3, &statusCible4, &statusCible5, &statusCible6, &statusCible7, 
                        &statusCible8, &statusCible9, &statusCible10};    
    int pins[] = {boutonE, boutonG, boutonD, cible1, cible2, cible3, cible4, cible5, cible6, cible7, cible8, cible9, cible10};
    // Acquisition des données des capteurs
    for (int i = 0; i < sizeof(pins) / sizeof(pins[0]); i++) {
        *(capteurs[i]) = digitalRead(pins[i]);
    }
}

void Temporisation(){
  for (int T = 1; T <= 100 ; T++){
    while(statusBoutonE==LOW ||statusBoutonD==LOW || statusBoutonG==LOW || statusCible1==LOW || statusCible2==LOW || statusCible3==LOW || statusCible4==LOW || statusCible5==LOW || statusCible6==LOW || statusCible7==LOW || statusCible8==LOW || statusCible9==LOW || statusCible10==LOW ){    
      AcquisitionCapteurs();
      delay(125);
      delay(delaiTemporisation);
    }
  }
}

void resetAllBonus0() {
    // Réinitialise tous les bonus
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus5() {
    // Réinitialise tous les bonus sauf pointBonus5
    int* allBonuses[] = {pointBonus10, pointBonus15, pointBonus25, pointBonus50, 
                         pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus10() {
    // Réinitialise tous les bonus sauf pointBonus10
    int* allBonuses[] = {pointBonus5, pointBonus15, pointBonus25, pointBonus50, 
                         pointBonus100, pointBonus150, pointBonus200, pointBonus250};
for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus15() {
    // Réinitialise tous les bonus sauf pointBonus15
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus25, pointBonus50, 
                         pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus25() {
    // Réinitialise tous les bonus sauf pointBonus25
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus50, 
                         pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus50() {
    // Réinitialise tous les bonus sauf pointBonus50
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus100() {
    // Réinitialise tous les bonus sauf pointBonus100
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus150() {
    // Réinitialise tous les bonus sauf pointBonus150
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus100, pointBonus200, pointBonus250};
    for (int i = 0; sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus200() {
    // Réinitialise tous les bonus sauf pointBonus200
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus100, pointBonus150, pointBonus250};
    for (int i = 0; sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}
void resetAllBonus250() {
    // Réinitialise tous les bonus sauf pointBonus250
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus100, pointBonus150, pointBonus200};
    for (int i = 0; sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}

void resetAllBonusExcept(int bonusToKeep) {
    // Tableau contenant tous les bonus
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus100, pointBonus150, pointBonus200, pointBonus250};

    // Parcourir tous les bonus
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        if (bonusToKeep != (5 * (i + 1))) { // Exclure le bonus à conserver
            allBonuses[i][joueurEnCours] = 0;
        }
    }
}

void resetAllBonuses() {
    int* allBonuses[] = {pointBonus5, pointBonus10, pointBonus15, pointBonus25, 
                         pointBonus50, pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        allBonuses[i][joueurEnCours] = 0;
    }
}

void TTRST() {
    // Tableau contenant tous les bonus à réinitialiser
    int* allBonuses[] = {pointBonus0, pointBonus5, pointBonus10, pointBonus15, pointBonus25,
                         pointBonus50, pointBonus100, pointBonus150, pointBonus200, pointBonus250};
    // Réinitialisation des bonus
    for (int i = 0; i < sizeof(allBonuses) / sizeof(allBonuses[0]); i++) {
        for (int j = 0; j < 5; j++) { // Taille définie pour 5 joueurs
            allBonuses[i][j] = 0;  // Remise à zéro
        }
    }
    // Réinitialisation des autres variables liées aux scores
    for (int i = 0; i < 5; i++) {
        scores[i] = 0;
        tauxReussite[i] = 0;
        oldClassement[i] = 0;
        classement[i] = 0;
        pointBonus[i] = 0;
        dernierTempsTouch[i] = 0;
        premiereTouche[i] = true; // Réinitialisation à "première touche"
    }
    Serial.println("Scores réinitialisés !");
    Serial.println("Bonus réinitialisés !");
}

void SonsAleatoires() {
  if (disableSonAnimation || disableLedAnimation) { // Ne fait rien si désactivé
    return;
  }
  // Tableau contenant les numéros des fichiers audio
  const int audioFiles[] = {35, 36, 87, 38, 39, 40, 41, 42, 43, 44, 
                            45, 46, 47, 48, 49, 24, 25, 26, 28};
  // Sélection d'un fichier audio aléatoire
  randomSeed(analogRead(12));
  int Son_aleatoire = random(0, sizeof(audioFiles) / sizeof(audioFiles[20]));
  // Lecture du fichier audio si le délai est respecté
  if (millis() - Time18M >= Period18M) {
    myDFPlayer.playMp3Folder(audioFiles[Son_aleatoire]);
    Time18M = millis();
  }
}

void Messages() {
  lcd.setCursor(0, 2);
  randomSeed(analogRead(11));
  int Ms_aleatoire = random(0, 11);
  // Tableau de messages sans utilisation de F()
  const char* messages[] = {
    "     Bah, alors     ",
    "  Ouvres les yeux   ",
    "     Tu louches     ",
    "   Vise la cible    ",
    "   Viser la lune    ",
    "      Tu dors       ",
    "    Tu es partie    ",
    "     Tu es mort     ",
    "   Tu es en enfer   ",
    "   Reveilles toi    "
  };
  // Vérification du temps et affichage du message
  if (millis() - Time2S >= Period2S) {
    lcd.print(messages[Ms_aleatoire]); // Affichage direct sans F()
    Time2S = millis();
  }
}
