class ScoreHistoryService {
  final List<Map<String, dynamic>> _scoreHistory = [];

  // Ajouter un score à l'historique
  void addScore(Map<String, dynamic> score) {
    _scoreHistory.add(score);
  }

  // Récupérer l'historique complet des scores
  List<Map<String, dynamic>> getScoreHistory() {
    return List.unmodifiable(_scoreHistory);
  }

  // Supprimer l'historique des scores
  void clearHistory() {
    _scoreHistory.clear();
  }
}