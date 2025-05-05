import 'package:flutter/material.dart';

class ScoresScreen extends StatelessWidget {
  const ScoresScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    // Liste des joueurs et des scores
    final List<Map<String, dynamic>> scores = [
      {'joueur': 'J1', 'score': 0, 'color': Colors.blue.shade200},
      {'joueur': 'J2', 'score': 0, 'color': Colors.blue.shade300},
      {'joueur': 'J3', 'score': 0, 'color': Colors.blue.shade400},
      {'joueur': 'J4', 'score': 0, 'color': Colors.blue.shade500},
    ];

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white, // Couleur de fond de l'AppBar
        elevation: 0, // Supprime l'ombre de l'AppBar
        leading: IconButton(
          icon: const Icon(Icons.arrow_back, color: Colors.black),
          onPressed: () {
            Navigator.pop(context); // Retour à la page précédente
          },
        ),
        title: const Text(
          'Scores',
          style: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.bold,
            color: Colors.black, // Couleur du texte de l'AppBar
          ),
        ),
        centerTitle: true, // Centrer le titre
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: LayoutBuilder(
          builder: (context, constraints) {
            // Calcule la hauteur des cases pour qu'elles soient rectangulaires
            final double itemHeight = (constraints.maxHeight - 48) / 2; // 48 = espacement vertical total
            final double itemWidth = (constraints.maxWidth - 32) / 2; // 32 = espacement horizontal total

            return GridView.builder(
              gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                crossAxisCount: 2, // 2 colonnes
                crossAxisSpacing: 16, // Espacement horizontal entre les cases
                mainAxisSpacing: 16, // Espacement vertical entre les cases
                childAspectRatio: itemWidth / itemHeight, // Ratio largeur/hauteur rectangulaire
              ),
              itemCount: scores.length,
              itemBuilder: (context, index) {
                final joueur = scores[index]['joueur'] as String;
                final score = scores[index]['score'] as int;
                final color = scores[index]['color'] as Color;

                return Container(
                  decoration: BoxDecoration(
                    gradient: LinearGradient(
                      colors: [
                        color.withOpacity(0.8),
                        color,
                      ],
                      begin: Alignment.topLeft,
                      end: Alignment.bottomRight,
                    ),
                    borderRadius: BorderRadius.circular(20), // Coins arrondis
                    boxShadow: [
                      BoxShadow(
                        color: Colors.black.withOpacity(0.1), // Ombre légère
                        blurRadius: 8,
                        offset: const Offset(0, 4),
                      ),
                    ],
                  ),
                  child: Center(
                    child: Column(
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        Text(
                          joueur,
                          style: const TextStyle(
                            fontSize: 72, // Taille pour le nom du joueur
                            fontWeight: FontWeight.bold,
                            color: Colors.white, // Couleur du texte
                          ),
                        ),
                        const SizedBox(height: 12), // Espacement entre les textes
                        Text(
                          '$score',
                          style: const TextStyle(
                            fontSize: 72, // Taille pour le score (identique à "Jx")
                            fontWeight: FontWeight.bold,
                            color: Colors.white, // Couleur du texte
                          ),
                        ),
                      ],
                    ),
                  ),
                );
              },
            );
          },
        ),
      ),
    );
  }
}