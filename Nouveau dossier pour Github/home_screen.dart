import 'package:flutter/material.dart';
import 'classement_screen.dart';
import 'scores_screen.dart';
import 'stats_screen.dart';

class HomeScreen extends StatelessWidget {
  const HomeScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white, // Couleur de fond de l'AppBar
        elevation: 0, // Supprime l'ombre de l'AppBar
        title: const Text(
          'Bienvenue sur la Cible Électronique 2.0', // Texte ajouté dans l'AppBar
          style: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.bold,
            color: Colors.black, // Couleur du texte de l'AppBar
          ),
        ),
        actions: [
          _buildStyledButton(
            label: 'Scores',
            emoji: '📊', // Émoticône pour les scores
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ScoresScreen()),
              );
            },
          ),
          _buildStyledButton(
            label: 'Classement',
            emoji: '🏆', // Émoticône pour le classement
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ClassementScreen()),
              );
            },
          ),
          _buildStyledButton(
            label: 'Stats',
            emoji: '📈', // Émoticône pour les statistiques
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const StatsScreen()),
              );
            },
          ),
        ],
      ),
      body: Container(), // Suppression du texte au milieu de la page
    );
  }

  Widget _buildStyledButton({
    required String label,
    required String emoji,
    required VoidCallback onPressed,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 5, vertical: 8), // Espacement autour des boutons
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
          foregroundColor: Colors.white, // Couleur du texte (blanc)
          backgroundColor: Colors.blue, // Couleur de fond (bleu)
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20), // Coins arrondis
          ),
          padding: const EdgeInsets.symmetric(horizontal: 15, vertical: 10), // Taille du bouton
        ),
        child: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            Text(
              emoji,
              style: const TextStyle(fontSize: 16), // Taille de l'emoji
            ),
            const SizedBox(width: 5), // Espacement entre l'emoji et le texte
            Text(
              label,
              style: const TextStyle(fontSize: 14, fontWeight: FontWeight.bold),
            ),
          ],
        ),
      ),
    );
  }
}