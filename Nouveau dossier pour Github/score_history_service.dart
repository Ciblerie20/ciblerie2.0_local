import 'package:flutter/foundation.dart';

class ScoreHistoryService with ChangeNotifier {
  final List<List<int>> _history = [];
  List<int>? _currentGame; // Partie en cours (scores des joueurs)

  List<List<int>> get history => _history;
  List<int>? get currentGame => _currentGame;

  // Ajouter un enregistrement des scores (ex: [10, 5, 3, 8])
  void addScores(List<int> scores) {
    if (scores.length != 4) {
      throw ArgumentError("Les scores doivent contenir exactement 4 joueurs.");
    }
    _history.add(List.from(scores)); // Copie défensive
    notifyListeners();
    _logMessage('📊 Nouvelle partie enregistrée : $scores');
  }

  // Mettre à jour les scores de la partie en cours
  void updateCurrentGame(List<int> scores) {
    if (scores.length != 4) {
      throw ArgumentError("Les scores doivent contenir exactement 4 joueurs.");
    }
    _currentGame = List.from(scores); // Copie défensive
    notifyListeners();
    _logMessage('🔄 Partie en cours mise à jour : $scores');
  }

  // Enregistrer la partie en cours dans l'historique
  void saveCurrentGame() {
    if (_currentGame != null) {
      addScores(_currentGame!);
      _currentGame = null; // Réinitialiser la partie en cours
      _logMessage('✅ Partie en cours enregistrée dans l’historique.');
    } else {
      _logMessage('⚠️ Aucune partie en cours à enregistrer.');
    }
  }

  // Récupérer les derniers scores enregistrés
  List<int>? getLastGame() {
    if (_history.isNotEmpty) {
      return _history.last;
    }
    return null;
  }

  // Réinitialiser les scores d’un joueur spécifique dans l’historique
  void resetPlayerScoreInHistory(int playerIndex) {
    for (var entry in _history) {
      if (playerIndex < entry.length) {
        entry[playerIndex] = 0;
      }
    }
    notifyListeners();
    _logMessage('🔄 Score du joueur ${playerIndex + 1} remis à zéro dans l’historique.');
  }

  // Supprimer tout l’historique des parties
  void clearHistory() {
    _history.clear();
    _currentGame = null; // Réinitialiser la partie en cours
    notifyListeners();
    _logMessage('🗑️ Historique des parties supprimé.');
  }

  // Méthode privée pour journaliser les messages
  void _logMessage(String message) {
    if (kDebugMode) {
      // Affiche uniquement en mode debug
      print(message);
    }
  }
}