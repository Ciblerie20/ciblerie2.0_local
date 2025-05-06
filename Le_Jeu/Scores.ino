

// Nouvelle partie à inclure dans "BaseScoresFin()" ,
// pour que le nouveau meilleur score (nouveau record) soit envoie vers esp32 
// qui lui l'envoie vers le site web = "scores-web.onrender.com"
/*
void BaseScoresFin() {
    // Exemple de traitement pour déterminer le meilleur joueur
    String pseudoMeilleurJoueur = pseudoJoueur[classement[1]]; // Pseudo du premier joueur
    int scoreMeilleurJoueur = scores[classement[1]]; // Score du premier joueur

    // Envoi des données au format attendu via Serial1
    Serial1.print("SCORE:");
    Serial1.print(pseudoMeilleurJoueur); // Pseudo du 1er
    Serial1.print(":");
    Serial1.println(scoreMeilleurJoueur); // Score du 1er

    // Affichage pour débogage
    Serial.println("Données envoyées via Serial1 :");
    Serial.print("SCORE:");
    Serial.print(pseudoMeilleurJoueur);
    Serial.print(":");
    Serial.println(scoreMeilleurJoueur);
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------------
// ----------------AFFICHAGE FIN DE PARTIE
//----------------------------------------------------------------------------------------------------------------------------------------------------------


//Fonction tri  bulle pour le classement des joueurs en fin de partie
void triclassement(){
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
            //Serial.print ("S1=");
            //Serial.print( classement[1] );
            //Serial.print ("=S2=");
            //Serial.print( classement[2] );
            //Serial.print ("=S3=");
            //Serial.print( classement[3] );
            //Serial.print ("=S4=");
            //Serial.println( classement[4] );
            } // fin if
          } // fin for
    } 
  while (classement[0]==1); // tant qu'on a fait une permutation, on recommence.
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// affichageScoresJeu

void affichageScoresJeu(int a){
 FastLED.clear();
 EcranScores1();
 lcd.clear();
 for (int i=0;i<=3;i++){
   lcd1.setCursor(0,i);
   lcd1.print(tabScores[a]);   
   a++;   
   if (a>4){a=0;}
 }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// ScoresFin (La base)

void BaseScoresFin() {
    lcd.clear();
    lcd1.clear();
    sortie = true; // 🚨 Problème - Doit être réinitialisé 
    initialisation = false;
    int indexScrollScore = 0;
    unsigned long currentMillis = millis();
    unsigned long previousMillis = millis();
    unsigned long period = 600000;  // Intervalle de temps pour faire défiler les scores

    NewScoresFin();
    NewPreaffichageScores(indexScrollScore);
 
    currentMillis = millis();      
    // Vérification du délai pour faire défiler les scores
    if (currentMillis - previousMillis >= period) {
        previousMillis = currentMillis;
        NewPreaffichageScores(indexScrollScore);
        indexScrollScore++;
        // Réinitialisation de l'index pour recommencer à afficher depuis le début
        if (indexScrollScore > 0) {
            indexScrollScore = 0;
        }
    }
    // Affichage et stockage des scores
    tabScores[0] = ("1ER : J" + String(classement[1]) + " : " + String(scores[classement[1]]));
//    tabScores[0] = "1ER : " + pseudoJoueur[classement[1]] + " : " + String(scores[classement[1]]);
    Serial.println("1ER : J" + String(classement[1]) + " : " + String(scores[classement[1]]));
    ResultatDesScores.println("1ER : J" + String(classement[1]) + " : " + String(scores[classement[1]]));    
    Serial.print("\t");  
    tabScores[1] = ("2EM : J" + String(classement[2]) + " : " + String(scores[classement[2]]));
    Serial.println("2EM : J" + String(classement[2]) + " : " + String(scores[classement[2]]));
    ResultatDesScores.println("2EM : J" + String(classement[2]) + " : " + String(scores[classement[2]]));    
    Serial.print("\t");  
    tabScores[2] = ("3EM : J" + String(classement[3]) + " : " + String(scores[classement[3]]));
    Serial.println("3EM : J" + String(classement[3]) + " : " + String(scores[classement[3]]));
    ResultatDesScores.println("3EM : J" + String(classement[3]) + " : " + String(scores[classement[3]]));    
    Serial.print("\t"); 
    tabScores[3] = ("4EM : J" + String(classement[4]) + " : " + String(scores[classement[4]]));
    Serial.println("4EM : J" + String(classement[4]) + " : " + String(scores[classement[4]]));
    ResultatDesScores.println("4EM : J" + String(classement[4]) + " : " + String(scores[classement[4]]));

    // Récupération des informations pour le score du meilleur joueur actuel
    String groupeActuel = groupes[indexgroupe];
    // Chargement du meilleur score depuis le fichier correspondant au groupe
    String fichierScore = groupeActuel + ".csv";

    // esp32 - WebSocket
    // Récupération du meilleur joueur
    String pseudoMeilleurJoueur = pseudoJoueur[classement[1]];
    int scoreMeilleurJoueur = scores[classement[1]];
    // Envoi des données au format JSON via Serial1
    String jsonMessage = "{\"pseudo\": \"" + pseudoMeilleurJoueur + "\", \"score\": " + String(scoreMeilleurJoueur) + "}";
    Serial1.println(jsonMessage); // Envoi au WebSocket via Serial1
    // Affichage pour vérification
    Serial.println("Données envoyées via Serial1 :");
    Serial.println(jsonMessage);

//  String fichierScore = String(groupes[indexgroupe]) + ".csv";
    int meilleurScore = extraireMeilleurScore(fichierScore.c_str());
    if (meilleurScore != -1) {
        Serial.println("🎯 Score à battre : " + String(meilleurScore));
    } else {
        Serial.println("⚠️ Aucun score valide trouvé !");
    }

    int scoreActuel = scores[classement[1]];  // Score du 1er joueur   
    if (scoreActuel > meilleurScore) {
      Serial.println("🏆 Nouveau record !");
      nouveauRecord = true; // ✅ Le record est battu
/*
  // Exemple de score à envoyer
  int nouveauRecord = 42;  // À remplacer par ta logique
  envoyerScoreMS1(nouveauRecord);
*/
      // ➕ Demande pseudo et enregistre dans le classement
      String nouveauPseudo = CreationPseudo(classement[1]);
      pseudoJoueur[classement[1]] = nouveauPseudo;
      enregistrerScore(scoreActuel, nouveauPseudo);

      meilleurScore = scoreActuel;

      String messageScore = "1ER : " + nouveauPseudo + " : " + String(scoreActuel);
      tabScores[0] = messageScore;
      Serial.println(messageScore);
      ResultatDesScores.println(messageScore);
      lcd1.setCursor(0, 0);
      lcd1.print(" ! Nouveau Record ! ");
      lcd1.setCursor(0, 1);
      lcd1.print("Score : " + String(meilleurScore));
      Serial.println("🎉 Nouveau Meilleur Score enregistré : " + String(meilleurScore));
    }
    else {
      nouveauRecord = false; // ✅ Aucun nouveau record
      Serial.println("⏳ Score insuffisant, passage à AfficheFin...");
      ClotureFin();
      Serial.println("🔹 Sortie de BaseScoresFin après AfficheFin()");
      return;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// NewScoresFin

// TABLEAU DE SCORES
void NewScoresFin(){
 lcd.clear();
 lcd1.clear();
 Serial.print("\t");
 tabScores[0] = ("1ER : J" + String(classement[1]) + " : " + String(scores[classement[1]]));
// tabScores[0] = "1ER : " + pseudoJoueur[classement[1]] + " : " + String(scores[classement[1]]);
 Serial.println("1ER : J" + String(classement[1]) + " : " + String(scores[classement[1]]));
 ResultatDesScores.println("1ER : J" + String(classement[1]) + " : " + String(scores[classement[1]]));
 Serial.print("\t");  
 tabScores[1]=("2EM : J" + String(classement[2])+ " : " + String(scores[classement[2]]));
 Serial.println("2EM : J" + String(classement[2])+ " : " + String(scores[classement[2]]));
 ResultatDesScores.println("2EM : J" + String(classement[2])+ " : " + String(scores[classement[2]]));
 Serial.print("\t");  
 tabScores[2]=("3EM : J" + String(classement[3])+ " : " + String(scores[classement[3]]));
 Serial.println("3EM : J" + String(classement[3])+ " : " + String(scores[classement[3]]));
 ResultatDesScores.println("3EM : J" + String(classement[3])+ " : " + String(scores[classement[3]]));
 Serial.print("\t"); 
 tabScores[3]=("4EM : J" + String(classement[4])+ " : " + String(scores[classement[4]]));
 Serial.println("4EM : J" + String(classement[4])+ " : " + String(scores[classement[4]]));
 ResultatDesScores.println("4EM : J" + String(classement[4])+ " : " + String(scores[classement[4]]));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// NewPreaffichageScores

void NewPreaffichageScores(int a){
 FastLED.clear();
 lcd.clear();
 for (int i=0;i<=3;i++){
   lcd.setCursor(0,i);
   lcd.print(tabScores[a]);   
   a++;   
   if (a>4){a=0;}
 }
 lcd1.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// code fournis ..................................

void testEnvoiESP32() {
    String pseudo = "JoueurTest";
    int score = 999;
    
    String jsonMessage = "{\"pseudo\": \"" + pseudo + "\", \"score\": " + String(score) + "}";
    Serial1.println(jsonMessage); // Envoi au WebSocket via Serial1

    // Affichage pour vérification
    Serial.println("Test - Données envoyées via Serial1 :");
    Serial.println(jsonMessage);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// extraireMeilleurScore

// ✅ Fonction améliorée pour extraire un score d'une ligne
int extraireScore(String ligne) {
    ligne.trim();  // Supprime les espaces ou retours à la ligne parasites
    int virgule = ligne.indexOf(',');

    if (virgule == -1) { 
        Serial.println("⚠️ Format incorrect : " + ligne);
        return -1; // Indicateur d'erreur au lieu de retourner 0
    }

    String scoreStr = ligne.substring(virgule + 1);
    scoreStr.trim();  // Supprime les espaces autour du score

    if (scoreStr.length() == 0 || !isdigit(scoreStr[0])) { 
        Serial.println("⚠️ Score invalide : " + ligne);
        return -1; // Retourne une erreur si le score est mal formaté
    }

    return scoreStr.toInt();  // Convertit et retourne le score en entier
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// extraireMeilleurScore

// ✅ Fonction améliorée pour extraire le meilleur score d'un fichier
int extraireMeilleurScore(const char* fichierNom) {
    File myFile = SD.open(fichierNom, FILE_READ);
    if (!myFile) {
        Serial.println("❌ Erreur : Impossible d'ouvrir le fichier " + String(fichierNom) + " !");
        return -1; // Retourne -1 en cas d'erreur
    }

    int meilleurScore = 0;
    while (myFile.available()) {
        String ligne = myFile.readStringUntil('\n');
        
        // Chercher le séparateur " : "
        int indexSeparateur = ligne.indexOf(" : ");
        if (indexSeparateur != -1) {
            String scoreStr = ligne.substring(indexSeparateur + 3);  // Prendre la partie après " : "
            int score = scoreStr.toInt();
            
            if (score > meilleurScore) {
                meilleurScore = score;
            }
        }
    }
    myFile.close();
    Serial.println("📂 Meilleur score extrait depuis le fichier : " + String(meilleurScore));
    return meilleurScore;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// verifierNouveauMeilleurScore

// ✅ Fonction pour vérifier si un nouveau meilleur score a été obtenu
bool verifierNouveauMeilleurScore() {
    // ✅ Récupérer le fichier du groupe actuel
    String fichierScore = String(groupes[indexgroupe]) + ".csv";

    // ✅ Extraire le meilleur score existant
    int meilleurScore = extraireMeilleurScore(fichierScore.c_str());
    Serial.println("🎯 Score à battre : " + String(meilleurScore));

    // ✅ Vérifier si le score du premier joueur est supérieur
    int scoreActuel = scores[classement[1]];
    Serial.println("🏆 Score du joueur actuel : " + String(scoreActuel));

    return (scoreActuel > meilleurScore);  // ✅ Retourne vrai si le score est meilleur
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// initialiserMeilleurScore 

void initialiserMeilleurScore() {
    // ✅ Récupérer le fichier de scores du groupe actuel & Fonction pour initialiser et extraire le meilleur score actuel
    String fichierScore = String(groupes[indexgroupe]) + ".csv"; 

    // ✅ Extraire le meilleur score depuis le fichier
    meilleurScore = extraireMeilleurScore(fichierScore.c_str());   // Lecture du fichier

    // 🔍 Affichage pour vérification
    Serial.println("🎯 Meilleur score initialisé à : " + String(meilleurScore));
    Serial.println("🎯 Score à battre : " + String(meilleurScore));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// AfficheFin (ClotureFin)

void ClotureFin(){
  sortie = true; // au départ, on bloque le menu
  initialisation = false;

  int indexScrollScore = 0;
  unsigned long currentMillis = millis();
  unsigned long previousMillis = millis();
  unsigned long period = 2000;  // Intervalle pour faire défiler les scores

  NewScoresFin();
  affichageScores(indexScrollScore);

  // 🔁 On reste ici jusqu'à ce que le bouton E soit pressé
  while (true) {  
    currentMillis = millis();      

    // Mise à jour de l'affichage à intervalles réguliers
    if (currentMillis - previousMillis >= period) {
        previousMillis = currentMillis;
        affichageScores(indexScrollScore);
        indexScrollScore++;

        // Remise à zéro de l'index si besoin
        if (indexScrollScore > 0) {
            indexScrollScore = 0;
        }
    }

    AcquisitionCapteurs(); // pour mettre à jour les boutons
    bool statusBoutonE = digitalRead(boutonE) == LOW; // 🔄 mise à jour à chaque boucle

    Serial.print("État bouton E : ");
    Serial.println(statusBoutonE ? "PRESSE ✅" : "RELACHE ❌");

    if (statusBoutonE) {
      // Le joueur a appuyé sur E : on sort de ClotureFin()
      delay(500);  // anti-rebond
      Serial.println(F("🔽 Bouton E pressé, sortie de ClotureFin()"));
      initialisation = true;  // retour au menu autorisé
      sortie = false;         // débloquer while(sortie)
      delay(125);             // petite pause visuelle
      Serial.println("➡️ Appel pour entrer dans ModeMenuScreenMMMS0");
      TTRST();                // reset général
      break;                  // 🛑 sortir de la boucle et de la fonction
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// enregistrerScore

//************ Fonction enregistrerScore ****************//
// Cette fonction insère le nouveau record en Rang 1 (après l'entête)
// et décale les autres scores, en conservant 50 lignes (1 entête + 49 scores).
// Fonction d'enregistrement des scores

void enregistrerScore(int score, String pseudo) {
    Serial.println("📌 Vérif : groupes[indexgroupe] = " + String(groupes[indexgroupe]));
    
    if (indexgroupe < 0 || indexgroupe >= nombreGroupes) {
        Serial.println("❌ Erreur : indexgroupe invalide !");
        return;
    }

    // Construire le bon nom de fichier
    String fichierNom = String(groupes[indexgroupe]) + ".csv";
    Serial.println("📂 Enregistrement du score dans : " + fichierNom);
    Serial.println("📌 Vérif : fichier utilisé = " + fichierNom);

    // Vérifier si le fichier existe
    File myFile = SD.open(fichierNom, FILE_READ);
    if (!myFile) {
        Serial.println("⚠️ Le fichier " + fichierNom + " n'existe pas. Création en cours...");

        // Créer le fichier s'il n'existe pas
        myFile = SD.open(fichierNom, FILE_WRITE);
        if (myFile) {
            myFile.println("Pseudo,Score"); // Entête du fichier CSV
            myFile.close();
            Serial.println("✅ Fichier " + fichierNom + " créé !");
        } else {
            Serial.println("❌ Erreur : Impossible de créer " + fichierNom);
            return;
        }
    }
    myFile.close();

    // Ajouter le nouveau score avec le format "J1 : 25"
    myFile = SD.open(fichierNom, FILE_WRITE);
    if (myFile) {
        myFile.println(pseudo + " : " + String(score)); // Formater comme "J1 : 25"
        myFile.close();
        Serial.println("✅ Score enregistré dans " + fichierNom);
    } else {
        Serial.println("❌ Erreur d'écriture sur la carte SD !");
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// affichageScores

void affichageScores(int a){
  FastLED.clear();
  lcd.clear();
  // Affichage des autres scores
  for (int i = 0; i <= 3; i++) {
    lcd.setCursor(0, i);
    lcd.print(tabScores[a]);
    a++;
    if (a > 4) { 
      a = 0; 
    }
  }
  if (nouveauRecord) {
    lcd1.setCursor(1, 0);
    lcd1.print("! Nouveau record !");
    lcd1.setCursor(0, 1);
    lcd1.print(pseudoJoueur[classement[1]] + " : " + String(scores[classement[1]]));
  }
  lcd1.setCursor(8, 3);
  lcd1.print(F("Start --> OK"));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// getNomJoueur

String getNomJoueur(int index) {
  if (pseudos[index] != "") {
    return pseudos[index];
  }
  return "J" + String(index);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// afficherContenuFichier

//************ Fonction afficherContenuFichier ****************//
// Affiche le contenu d'un fichier sur le moniteur série
// 📖 Fonction pour afficher le contenu d'un fichier
void afficherContenuFichier(String fichierNom) {
  File file = SD.open(fichierNom, FILE_READ);
  if (!file) {
    Serial.println("❌ Erreur : Impossible d'ouvrir " + fichierNom + " en lecture !");
    return;
  }
  Serial.println("📂 Contenu de " + fichierNom + " :");
  
  while (file.available()) {
    Serial.write(file.read());
  }
  file.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// afficherScores

//************ Fonction afficherScores ****************//
// Cette fonction lit le fichier du groupe courant et affiche le classement sur les LCD.
// On utilise une boucle externe pour recharger les données afin que,
// en cas de changement de groupe (statusBoutonG ou D), le nouveau groupe soit relu,
// sans renvoyer directement à l'accueil.

void afficherScores() {
  const int maxScores = 50;
  String lignes[maxScores];
  int totalScores = 0;
  int indexAffichage = 0;

  while (true) {
    totalScores = 0;

    // --- Lecture du fichier du groupe courant ---
    String fichierNom = String(groupes[indexgroupe]) + ".csv";
    File fichier = SD.open(fichierNom, FILE_READ);

    if (fichier) {
      while (fichier.available() && totalScores < maxScores) {
        String ligne = "";
        char c;
        while (fichier.available()) {
          c = fichier.read();
          if (c == '\n' || c == '\r') break;
          ligne += c;
        }
        ligne.trim();
        if (ligne.length() > 0) {
          lignes[totalScores++] = ligne;
        }
      }
      fichier.close();

      // --- Tri décroissant des scores ---
      for (int i = 0; i < totalScores - 1; i++) {
        for (int j = i + 1; j < totalScores; j++) {
          int score_i = lignes[i].substring(lignes[i].lastIndexOf(':') + 1).toInt();
          int score_j = lignes[j].substring(lignes[j].lastIndexOf(':') + 1).toInt();
          if (score_j > score_i) {
            String temp = lignes[i];
            lignes[i] = lignes[j];
            lignes[j] = temp;
          }
        }
      }
    } else {
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0, 0);
      lcd.print("Fichier manquant:");
      lcd.setCursor(0, 1);
      lcd.print(fichierNom);
      delay(2000);
      return; // on quitte la fonction si fichier introuvable
    }

    // --- Affichage sur LCD principal ---
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Groupe: " + String(groupes[indexgroupe]));
    lcd.setCursor(1, 2);
    lcd.print("! Score a battre !");
    if (totalScores > 0) {
      lcd.setCursor(0, 3);
      lcd.print(lignes[0].substring(0, 16)); // meilleur score
    }

    // --- Affichage sur LCD secondaire ---
    lcd1.clear();
    for (int i = 0; i < 4; i++) {
      int ligneIndex = indexAffichage + i + 1;
      if (ligneIndex < totalScores) {
        // Récupération et découpage
        String ligne = lignes[ligneIndex];
        String pseudo = ligne.substring(0, ligne.indexOf(":"));
        String score = ligne.substring(ligne.indexOf(":") + 1);

        // Nettoyage du pseudo : supprimer les ':' s'il y en a par erreur
        pseudo.replace(":", "");
        if (pseudo.length() > 15) pseudo = pseudo.substring(0, 15);

        // Format rang (ex: "2.")
        String rang = String(ligneIndex + 1) + ".";
        lcd1.setCursor(0, i);
        lcd1.print(rang);

        // Affichage pseudo
        lcd1.setCursor(3, i);
        lcd1.print(pseudo);

        // Affichage séparateur ':'
        lcd1.setCursor(12, i);
        lcd1.print(":");

        // Affichage score aligné à droite
        int scoreLength = score.length();
        lcd1.setCursor(19 - scoreLength + 1, i); // 20 colonnes, index 0–19
        lcd1.print(score);
      }
    }

    // --- Boucle de navigation / interaction ---
    bool attendreNouvelInput = true;
    while (attendreNouvelInput) {
      int ButtonCode = read_LCD_buttons();
      bool statusBoutonG = digitalRead(boutonG) == LOW;
      bool statusBoutonD = digitalRead(boutonD) == LOW;

      if (ButtonCode == btnUP && indexAffichage > 0) {
        indexAffichage--;
        attendreNouvelInput = false;
      }

      if (ButtonCode == btnDOWN && indexAffichage + 5 < totalScores) {
        indexAffichage++;
        attendreNouvelInput = false;
      }

      if (ButtonCode == btnSELECT) {
        lcd.clear();
        lcd1.clear();
        afficherMenu = false;  // On quitte simplement la boucle
        return; // Quitter proprement, l'appelant peut rappeler MenuScreen0() et // Retour vers loop() qui gère le reste
      }

      if (statusBoutonG) {
        indexgroupe = (indexgroupe + 1) % nombreGroupes;
        indexAffichage = 0;
        attendreNouvelInput = false;
      }

      if (statusBoutonD) {
        indexgroupe = (indexgroupe - 1 + nombreGroupes) % nombreGroupes;
        indexAffichage = 0;
        attendreNouvelInput = false;
      }

      delay(150); // anti-rebond
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// --------- PARTIE CONCERNANT LE PSEUDO ----------

///////////////////////////////////////////////////////////////////////////////////////////////////////// CreationPseudo
/*

Pour la création du Pseudo :
En tenant compte des 20 cases du LCD :
A voir car la case "14" est vides ?

Peux-tu arranger l'affichage en respectant cela ?

Rang    .  Pseudo             :         Score
2 à 50                                  Maximun = 999.999

1 2     3  4 5 6 7 8 9 10 11  12  13 14 15 16 17 18 19 20
50      .  F R A a 1 # A a 1  #   :     9  9  9  9  9  9

50      .  A a 1 # A a 1  #   :         9  9  9  9  9  9

           F R A a 1 # A a 1  #   :
           
FR = France mettre drapeau

1 2 3 4 5 6 7 8 - Caractères [8]
A a 1 # A a 1 #

*/
//************ Fonction CreationPseudo ****************//
// Gère la saisie du pseudo pour un nouveau record.
String CreationPseudo(int indexCible) {
  Serial.println("📊 DEBUG: Début de CreationPseudo");

  lcd.clear();
  lcd1.clear();

  sortie = true;
  initialisation = false;
  modeSaisie = true;

  String nouveauPseudo = "";
  pseudoIndex = 0;

  while (modeSaisie) {
    AcquisitionCapteurs();
    ButtonCode = read_LCD_buttons();
    bool statusBoutonG = digitalRead(boutonG) == LOW;
    bool statusBoutonD = digitalRead(boutonD) == LOW;

    lcd.setCursor(0, 1);
    lcd.print("Tu es le ! WINNER !");  

    lcd1.setCursor(0, 0);
    lcd1.print("Pseudo: " + nouveauPseudo + " ");
    lcd1.setCursor(0, 1);
    lcd1.print("Caractere: ");
    lcd1.print(caracteres[pseudoIndex]);
    lcd1.print("  ");

    if (ButtonCode == btnUP) {
      pseudoIndex = (pseudoIndex + 1) % (sizeof(caracteres) / sizeof(caracteres[0]));
    }

    if (ButtonCode == btnDOWN) {
      pseudoIndex = (pseudoIndex - 1 + sizeof(caracteres) / sizeof(caracteres[0])) % (sizeof(caracteres) / sizeof(caracteres[0]));
    }

    if (statusBoutonD) {
      nouveauPseudo += caracteres[pseudoIndex];
      Serial.print("Ajout caractère: ");
      Serial.println(caracteres[pseudoIndex]);
    }

    if (statusBoutonG && nouveauPseudo.length() > 0) {
      nouveauPseudo.remove(nouveauPseudo.length() - 1);
      Serial.println("Suppression dernier caractère");
    }

    if (ButtonCode == btnSELECT) {
      if (nouveauPseudo.length() > 0) {
        File myFile = SD.open("pseudo.csv", FILE_WRITE);
        if (myFile) {
          myFile.println(nouveauPseudo);
          myFile.close();
        }

        Serial.println("✅ Pseudo enregistré : " + nouveauPseudo);
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print("Ajoute !");
        delay(1500);

        // 💾 Affecter le pseudo à l'index ciblé (classement[1])
        pseudoJoueur[indexCible] = nouveauPseudo;
        pseudos[totalPseudos] = nouveauPseudo;
        totalPseudos++;

        modeSaisie = false;
        sortie = true;
        initialisation = false;
        ClotureFin();
        lcd.clear();
        lcd1.clear();

        return nouveauPseudo;
      } else {
        lcd1.clear();
        lcd1.setCursor(0, 0);
        lcd1.print("Liste pleine!");
        Serial.println("Liste pleine, impossible d'ajouter");
        delay(1500);
        lcd1.clear();
      }
    }

    delay(200);
  }

  return "";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// sauvegarderPseudos

// 📂 **Sauvegarde des pseudos dans pseudo.txt**
void sauvegarderPseudos() {
  File myFile = SD.open("pseudo.csv", FILE_WRITE | O_TRUNC);
  if (myFile) {
    for (int i = 0; i < totalPseudos; i++) {
      myFile.println(pseudos[i]);
    }
    myFile.close();
  } else {
    lcd1.setCursor(0, 3);
    lcd1.print("Erreur SD !");
    delay(1500);
  }
  lireTousPseudos();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// lireTousPseudos

// 📂 **Lecture des pseudos depuis la carte SD**
void lireTousPseudos() {
  File myFile = SD.open("pseudo.csv", FILE_READ);
  if (!myFile) return;

  totalPseudos = 0;
  while (myFile.available()) {
    String pseudoTemp = myFile.readStringUntil('\n');
    pseudoTemp.replace("\r", "");
    pseudoTemp.replace("\n", "");
    if (pseudoTemp.length() > 0) { // Éviter les lignes vides
      pseudos[totalPseudos] = pseudoTemp;
      totalPseudos++;
      if (totalPseudos >= MAX_PSEUDOS) break;
    }
  }
  myFile.close();

  trierPseudos();  // Trie les pseudos après lecture
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// trierPseudos

// 🔧 **Fonction de tri des pseudos**
void trierPseudos() {
  for (int i = 0; i < totalPseudos - 1; i++) {
    for (int j = i + 1; j < totalPseudos; j++) {
      if (pseudos[i] > pseudos[j]) { // Comparaison alphabétique
        String temp = pseudos[i];
        pseudos[i] = pseudos[j];
        pseudos[j] = temp;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// afficherMenuPseudos

void afficherMenuPseudos() {
  static int dernierIndexAffichage = -1; // Garde en mémoire le dernier affichage pour éviter les réécritures inutiles

  // Si la page n'a pas changé, ne rien faire pour éviter les scintillements
  if (indexAffichage == dernierIndexAffichage) return;  
  dernierIndexAffichage = indexAffichage;

  lcd1.clear(); // On efface tout l'écran uniquement si un changement est détecté

  lcd1.setCursor(0, 0);
  lcd1.print("Liste Pseudos:");

  // Calcul des indices des pseudos à afficher
  int indexPrecedent = (indexAffichage - 1 + totalPseudos) % totalPseudos;  // Pseudo précédent
  int indexSuivant = (indexAffichage + 1) % totalPseudos;  // Pseudo suivant

  // Affichage des pseudos (précédent, actuel, suivant)
  lcd1.setCursor(2, 1);  
  lcd1.print(pseudos[indexPrecedent] + "     "); // Ajouter des espaces pour effacer l'ancien texte

  lcd1.setCursor(0, 2);  
  lcd1.print(">  " + pseudos[indexAffichage] + "     "); // Pseudo actuel avec flèche

  lcd1.setCursor(2, 3);  
  lcd1.print(pseudos[indexSuivant] + "     ");

  // Pause pour éviter trop d'affichages successifs et réduire les scintillements
  delay(100);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// gestionNavigation

void gestionNavigation() {
  ButtonCode = read_LCD_buttons();
  
  if (ButtonCode == btnUP) { // Navigation vers le haut
    indexAffichage = (indexAffichage > 0) ? indexAffichage - 1 : totalPseudos - 1;
    afficherMenuPseudos();
    delay(ButtonDelay);
  }

  if (ButtonCode == btnDOWN) { // Navigation vers le bas
    indexAffichage = (indexAffichage + 1) % totalPseudos;
    afficherMenuPseudos();
    delay(ButtonDelay);
  }

  if (digitalRead(boutonG) == LOW) { // Sélectionner un pseudo
    lcd1.setCursor(0, 3);
    lcd1.print("Select: ");
    lcd1.print(pseudos[indexAffichage]);
    confirmationSuppression = false;  // Annule la suppression en cours si on sélectionne un pseudo
    delay(1500);
    afficherMenuPseudos();
  }
  if (digitalRead(boutonD) == LOW) { // Demander confirmation avant suppression
    if (!confirmationSuppression) { 
      lcd1.setCursor(0, 3);
      lcd1.print("Confirmer ?");
      confirmationSuppression = true;  // Active le mode confirmation
      delay(1500);
      afficherMenuPseudos();
    } else { 
      supprimerPseudo(indexAffichage);
      confirmationSuppression = false;  // Réinitialisation après suppression
      afficherMenuPseudos();
      delay(ButtonDelay);
    }
  }
  if (ButtonCode == btnSELECT) { // Quitter
    lcd1.clear();  // Correction : Nettoyage avant de revenir en mode saisie
    lcd1.print("Sortie du menu");
    delay(1500);
    modeSaisie = true;
    return;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////// supprimerPseudo

// ❌ **Suppression d'un pseudo**
void supprimerPseudo(int index) {
  if (index >= 0 && index < totalPseudos) {
    // Décalage des pseudos après le pseudo supprimé
    for (int i = index; i < totalPseudos - 1; i++) {
      pseudos[i] = pseudos[i + 1];
    }
    totalPseudos--; // Réduire le nombre de pseudos

    // Recharger les pseudos sur la carte SD
    File myFile = SD.open("pseudo.csv", FILE_WRITE | O_TRUNC);
    if (myFile) {
      for (int i = 0; i < totalPseudos; i++) {
        myFile.println(pseudos[i]);  // Sauvegarder chaque pseudo dans le fichier
      }
      myFile.close();
    }

    // Mettre à jour l'affichage
    lcd1.setCursor(0, 3);
    lcd1.print("Supprime !");
    delay(1500);  // Attendre un peu avant de revenir au menu
    indexAffichage = 0;  // Réinitialiser l'affichage à l'index 0
    afficherMenuPseudos();  // Afficher le menu des pseudos mis à jour
  }
}


