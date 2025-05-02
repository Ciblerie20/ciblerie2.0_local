import 'package:flutter/material.dart';

class ScoreHistoryService with ChangeNotifier {
  final List<List<int>> _history = [];

  List<List<int>> get history => _history;

  // Ajouter un enregistrement des scores (ex: [10, 5, 3, 8])
  void addScores(List<int> scores) {
    _history.add(List.from(scores)); // Copie défensive
    notifyListeners();
    print('📊 Nouvelle partie enregistrée : $scores');
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
    print('🔄 Score du joueur ${playerIndex + 1} remis à zéro dans l’historique.');
  }

  // Supprimer tout l’historique des parties
  void clearHistory() {
    _history.clear();
    notifyListeners();
    print('🗑️ Historique des parties supprimé.');
  }
}
