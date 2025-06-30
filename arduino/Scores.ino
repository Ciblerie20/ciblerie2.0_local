
//----------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------AFFICHAGE FIN DE PARTIE
//----------------------------------------------------------------------------------------------------------------------------------------------------------

// TABLEAU DE SCORES
void ScoresFin(){
 Serial.println("\t");     
 Serial.print("\t");
 tabScores[0]=("1ER : J" + String(classement[1])+ " : " + String(scores[classement[1]]));
 Serial.println("1ER : J" + String(classement[1])+ " : " + String(scores[classement[1]]));
 Serial.print("\t");  
 tabScores[1]=("2EM : J" + String(classement[2])+ " : " + String(scores[classement[2]]));
 Serial.println("2EM : J" + String(classement[2])+ " : " + String(scores[classement[2]]));
 Serial.print("\t");  
 tabScores[2]=("3EM : J" + String(classement[3])+ " : " + String(scores[classement[3]]));
 Serial.println("3EM : J" + String(classement[3])+ " : " + String(scores[classement[3]]));
 Serial.print("\t"); 
 tabScores[3]=("4EM : J" + String(classement[4])+ " : " + String(scores[classement[4]]));
 Serial.println("4EM : J" + String(classement[4])+ " : " + String(scores[classement[4]]));
 Serial.print("\t");
}
  
void affichageScores(int a){
 FastLED.clear();
 lcd.clear();
 for (int i=0;i<=3;i++){
   lcd.setCursor(0,i);
   lcd.print(tabScores[a]);   
   a++;   
   if (a>4){a=0;}
 }
 lcd1.clear();
 lcd1.setCursor(8,3);
 lcd1.print(F("Start --> OK"));
}

void affichageScoresJeu(int a){
  FastLED.clear();
 lcd.clear();
 for (int i=0;i<=3;i++){
   lcd1.setCursor(0,i);
   lcd1.print(tabScores[a]);   
   a++;   
   if (a>4){a=0;}
 }
}

// --- Liste de tous tes groupes ---
const char* groupesDisponibles[] = {
  "CF1", "CF2", "CF3", "CF4",
  "CSP1", "CSP2", "CSP3", "CSP4",
  "CP1", "CP2", "CP3", "CP4",
  "CC1", "CC2", "CC3", "CC4",
  "CL1", "CL2", "CL3", "CL4",
  "LJ1", "LJ2", "LJ3", "LJ4",
  "MS1", "MS2", "MS3", "MS4",
  "CPT1"
};
// --- Fonction utilitaire ---
bool isGroupeValide(const String& nom) {
  for (unsigned int i = 0; i < sizeof(groupesDisponibles)/sizeof(groupesDisponibles[0]); ++i) {
    if (nom == groupesDisponibles[i]) return true;
  }
  return false;
}

void AfficheFin() {
  myDFPlayer.playMp3Folder(20);
  int indexScrollScore = 0;
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();
  unsigned long period = 2000;  
  Temporisation();
  ScoresFin();
  String nouveauGroupe = "";
  // Boucle d'attente de validation fin de partie (bouton E ou virtuel, ou réception nouveau groupe)
  while (statusBoutonE == HIGH && !boutonVirtuelE && nouveauGroupe == "") {
    currentMillis = millis();
    if (currentMillis - previousMillis >= period) {
      previousMillis = currentMillis;
      affichageScores(indexScrollScore);
      indexScrollScore++;
      if (indexScrollScore > 0) {
        indexScrollScore = 0;
      }
    }
    AcquisitionCapteurs();
    // ----- Traitement des commandes série reçues pendant la boucle -----
    if (Serial1.available() > 0) {
      String messageLu = Serial1.readStringUntil('\n');
      messageLu.trim();
      Serial.print("📥 Message reçu de l'ESP32 : ");
      Serial.println(messageLu);
      if (messageLu == "BOUTON_E_VIRTUEL") {
        boutonVirtuelE = true;
        Serial.println("Validation bouton E reçue à distance !");
      }
      // Nouveau groupe reçu à distance (tous les groupes gérés)
      else if (isGroupeValide(messageLu)) {
        nouveauGroupe = messageLu;
        Serial.print("Nouveau groupe reçu pendant fin de partie : ");
        Serial.println(nouveauGroupe);
      }
    }
  }
  delay(500);
  // ----- Cas 1 : validation classique (physique ou virtuelle) -----
  if (statusBoutonE == LOW || boutonVirtuelE) {
    AcquisitionCapteurs();
    initialisation = true;
    boutonVirtuelE = false; // reset pour la prochaine partie
    delay(125);
    return;
  }
  // ----- Cas 2 : nouveau groupe reçu à distance -----
  if (nouveauGroupe != "") {
    Serial.println("Réinitialisation : nouveau groupe, début d'une nouvelle partie !");
    initialisation = true;
    partieEnCours = false;
    partieFinie = false;
    boutonVirtuelE = false;
    groupeSelectionne = nouveauGroupe; // <-- AJOUTE CETTE LIGNE !
    Serial.print("Mise à jour groupeSelectionne : ");
    Serial.println(groupeSelectionne);
    // --- 1. On indique au front qu'on est prêt ---
    Serial1.println("START");
    Serial.print("📨 Envoi à ESP32/Front : START pour ");
    Serial.println(nouveauGroupe);
    // --- 2. On attend explicitement la commande START_GAME du front ---
    bool gameStarted = false;
    while (!gameStarted) {
      if (Serial1.available() > 0) {
        String messageStart = Serial1.readStringUntil('\n');
        messageStart.trim();
        Serial.print("📥 Message reçu de l'ESP32 (attente START_GAME) : ");
        Serial.println(messageStart);
        if (messageStart == "START_GAME") {
          Serial.println("Commande START_GAME reçue, lancement de la partie !");
          gameStarted = true;
        }
      }
      AcquisitionCapteurs();
      delay(10); // pour éviter de saturer la boucle
    }
    // --- 3. Appelle la fonction du groupe choisi ---
    if (nouveauGroupe == "CF1") CF1();
    else if (nouveauGroupe == "CF2") CF2();
    else if (nouveauGroupe == "CF3") CF3();
    else if (nouveauGroupe == "CF4") CF4();
    else if (nouveauGroupe == "CSP1") CSP1();
    else if (nouveauGroupe == "CSP2") CSP2();
    else if (nouveauGroupe == "CSP3") CSP3();
    else if (nouveauGroupe == "CSP4") CSP4();
    else if (nouveauGroupe == "CP1") CP1();
    else if (nouveauGroupe == "CP2") CP2();
    else if (nouveauGroupe == "CP3") CP3();
    else if (nouveauGroupe == "CP4") CP4();
    else if (nouveauGroupe == "CC1") CC1();
    else if (nouveauGroupe == "CC2") CC2();
    else if (nouveauGroupe == "CC3") CC3();
    else if (nouveauGroupe == "CC4") CC4();
    else if (nouveauGroupe == "CL1") CL1();
    else if (nouveauGroupe == "CL2") CL2();
    else if (nouveauGroupe == "CL3") CL3();
    else if (nouveauGroupe == "CL4") CL4();
    else if (nouveauGroupe == "LJ1") LJ1();
    else if (nouveauGroupe == "LJ2") LJ2();
    else if (nouveauGroupe == "LJ3") LJ3();
    else if (nouveauGroupe == "LJ4") LJ4();
    else if (nouveauGroupe == "MS1") MS1();
    else if (nouveauGroupe == "MS2") MS2();
    else if (nouveauGroupe == "MS3") MS3();
    else if (nouveauGroupe == "MS4") MS4();
    else if (nouveauGroupe == "CPT1") CPT1();
    delay(125);
    return;
  }
}

//---------------------------------------------------------------------------------------

//Fonction tri  bulle pour le classement des joueurs en fin de partie
void triclassement(){
//***********************************************************************************************
// classer classement[] = {0,0,0,0,0};int scores[] = {0,0,0,0,0};
// pour ne pas faire le tri plusieurs fois, sera mis  1  la fin du traitement, remis  0 au dpart de la partie suivante
// remplissage du tableau de classement
classement[1]=1;
classement[2]=2;
classement[3]=3;
classement[4]=4;
// tri  bulle
  do
    { // boucle do/while
    classement[0]=0; // sera utilis dans le tri comme flag de permutation
      for (int i = 1 ; i < 4 ; i++)
        { // on parcourt le tableau
          if (scores[classement[i]]<scores[classement[i+1]])
            { // si le score de l'lment est <  l'lment suivant, on permute
            classement[0]=classement[i];          // on stocke le premier lment
            classement[i]=classement[i+1];          // on remonte le suivant
            classement[i+1]=classement[0];          // on termine la permutation
            classement[0]=1;                // on passe le flag de permutation  1
            } // fin if
          } // fin for
    } 
  while (classement[0]==1); // tant qu'on a fait une permutation, on recommence.
//*******************************************************************************************************   
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
