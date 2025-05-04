import 'package:flutter/material.dart';

class ScoreHistoryService with ChangeNotifier {
  final List<List<int>> _history = [];

  List<List<int>> get history => _history;

  // Ajouter un enregistrement des scores (ex: [10, 5, 3, 8])
  void addScores(List<int> scores) {
    _history.add(List.from(scores)); // Copie défensive
    notifyListeners();
  }

  // Supprimer les scores d’un joueur à un index donné
  void resetPlayerScoreInHistory(int playerIndex) {
    for (var entry in _history) {
      if (playerIndex < entry.length) {
        entry[playerIndex] = 0;
      }
    }
    notifyListeners();
  }

  // Vider complètement l’historique
  void clearHistory() {
    _history.clear();
    notifyListeners();
  }
}
