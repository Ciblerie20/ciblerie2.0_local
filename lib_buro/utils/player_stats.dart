import 'package:hive/hive.dart';

@HiveType(typeId: 0)
class PlayerStats extends HiveObject {
  @HiveField(0)
  int gamesPlayed;

  @HiveField(1)
  int totalScore;

  @HiveField(2)
  int bestScore;

  @HiveField(3)
  String group;

  PlayerStats({
    this.gamesPlayed = 0,
    this.totalScore = 0,
    this.bestScore = 0,
    this.group = "A",
  });

  /// Met à jour les statistiques du joueur avec un nouveau score
  void update(int newScore) {
    if (newScore < 0) {
      throw ArgumentError("Le score ne peut pas être négatif.");
    }
    gamesPlayed++;
    totalScore += newScore;
    if (newScore > bestScore) {
      bestScore = newScore;
    }
  }

  /// Réinitialise les statistiques du joueur
  void reset() {
    gamesPlayed = 0;
    totalScore = 0;
    bestScore = 0;
    group = "A";
  }

  /// Retourne les statistiques sous forme de chaîne lisible
  @override
  String toString() {
    return 'PlayerStats(gamesPlayed: $gamesPlayed, totalScore: $totalScore, bestScore: $bestScore, group: $group)';
  }
}
