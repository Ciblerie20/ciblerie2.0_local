import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../services/score_history_service.dart';
import '../services/websocket_service.dart';

class StatsScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final historyService = Provider.of<ScoreHistoryService>(context);
    final websocketService = Provider.of<WebSocketService>(context);
    final List<List<int>> history = historyService.history;

    // Calcul des statistiques pour chacun des 4 joueurs.
    List<Map<String, dynamic>> playerStats = List.generate(4, (index) {
      List<int> scores = history.map((game) => game[index]).toList();
      int total = scores.fold(0, (prev, curr) => prev + curr);
      double average = scores.isNotEmpty ? total / scores.length : 0.0;
      int best = scores.isNotEmpty ? scores.reduce((a, b) => a > b ? a : b) : 0;

      // Calcul du nombre de fois que chaque score prédéfini a été touché.
      Map<int, int> scoreFrequency = {5: 0, 10: 0, 15: 0, 25: 0, 50: 0};

      // Mise à jour des fréquences avec les données locales dans history.
      for (int score in scores) {
        if (scoreFrequency.containsKey(score)) {
          scoreFrequency[score] = scoreFrequency[score]! + 1;
        }
      }

      // Incorporer les scores reçus via WebSocket
      final webSocketScore = websocketService.getScoreForBox(index)['score'];
      if (scoreFrequency.containsKey(webSocketScore)) {
        scoreFrequency[webSocketScore] =
            scoreFrequency[webSocketScore]! + 1; // Mise à jour fréquence.
      }

      return {
        'joueur': 'J${index + 1}',
        'parties': scores.length,
        'total': total,
        'moyenne': average.toStringAsFixed(2),
        'meilleur': best,
        'scores': scores,
        'frequency': scoreFrequency,
      };
    });

    return Scaffold(
      appBar: AppBar(
        title: Text("Statistiques"),
        actions: [
          IconButton(
            icon: Icon(Icons.delete_forever),
            onPressed: () {
              historyService.clearHistory();
              ScaffoldMessenger.of(context)
                  .showSnackBar(SnackBar(content: Text("🗑️ Historique supprimé")));
            },
          ),
        ],
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: SingleChildScrollView(
          child: Row(
            children: List.generate(4, (index) {
              return Expanded(
                child: Padding(
                  padding: const EdgeInsets.symmetric(horizontal: 8.0),
                  child: _buildPlayerCard(
                      context, index, playerStats[index], historyService),
                ),
              );
            }),
          ),
        ),
      ),
    );
  }

  Widget _buildPlayerCard(BuildContext context, int playerIndex,
      Map<String, dynamic> stat, ScoreHistoryService historyService) {
    return Card(
      elevation: 3,
      child: Padding(
        padding: const EdgeInsets.all(12.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            Row(
              children: [
                Expanded(
                  child: Text(
                    stat['joueur'],
                    style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                    overflow: TextOverflow.ellipsis,
                  ),
                ),
                SizedBox(width: 4),
                ElevatedButton.icon(
                  style: ElevatedButton.styleFrom(
                    backgroundColor: Colors.redAccent,
                    padding: EdgeInsets.symmetric(horizontal: 8, vertical: 4),
                  ),
                  onPressed: () {
                    historyService.resetPlayerScoreInHistory(playerIndex);
                    ScaffoldMessenger.of(context).showSnackBar(
                      SnackBar(
                        content: Text(
                            "Statistiques de ${stat['joueur']} réinitialisées"),
                      ),
                    );
                  },
                  icon: Icon(Icons.refresh, size: 16),
                  label: Text("Reset", style: TextStyle(fontSize: 12)),
                ),
              ],
            ),
            SizedBox(height: 8),
            Text("Parties jouées : ${stat['parties']}"),
            Text("Score total : ${stat['total']}"),
            Text("Moyenne : ${stat['moyenne']}"),
            Text("Meilleur score : ${stat['meilleur']}"),
            SizedBox(height: 8),
            Text(
              "Tableau des fréquences :",
              style: TextStyle(fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 4),
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: (stat['frequency'] as Map<int, int>).entries.map((entry) {
                return Padding(
                  padding: EdgeInsets.symmetric(vertical: 2),
                  child: Text(" ${entry.key} points : ${entry.value} "),
                );
              }).toList(),
            ),
            SizedBox(height: 8),
            Text(
              "Scores enregistrés :",
              style: TextStyle(fontWeight: FontWeight.bold),
            ),
            SizedBox(height: 4),
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: (stat['scores'] as List<int>).map((score) {
                return Padding(
                  padding: EdgeInsets.symmetric(vertical: 2),
                  child: Text(" $score"),
                );
              }).toList(),
            ),
          ],
        ),
      ),
    );
  }
}
