import 'package:flutter/material.dart';
import 'dart:convert';
import 'package:flutter/scheduler.dart';
import 'package:provider/provider.dart';
import '../services/score_history_service.dart';
import '../services/websocket_service.dart';
import '../models/group_model.dart';
import '../utils/open_url_stub.dart'
if (dart.library.html) '../utils/open_url_web.dart';
import '../utils/time_utils.dart';

class StatsScreen extends StatefulWidget {
  const StatsScreen({super.key});

  @override
  State<StatsScreen> createState() => _StatsScreenState();
}

class _StatsScreenState extends State<StatsScreen> with SingleTickerProviderStateMixin {
  late AnimationController _controller;
  late Animation<double> _fadeAnimation;
  late Animation<double> _scaleAnimation;

  final List<Color> greenGradient = [
    const Color(0xFFA8E6A3),
    const Color(0xFF8CD58B),
    const Color(0xFF6ACD72),
    const Color(0xFF4AB25A),
  ];

  final List<Map<String, dynamic>> _playerStats = List.generate(4, (index) => {
    'score': 0,
    'total': 0,
    'best': 0,
    'games': 0,
    'group': '-',
    'frequencies': {
      0: 0,
      5: 0,
      10: 0,
      15: 0,
      25: 0,
      50: 0,
      100: 0,
      150: 0,
      200: 0,
      250: 0,
    },
    'history': <Map<String, dynamic>>[],
  });

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(duration: const Duration(milliseconds: 500), vsync: this);
    _fadeAnimation = Tween<double>(begin: 0, end: 1).animate(_controller);
    _scaleAnimation = Tween<double>(begin: 0.95, end: 1).animate(_controller);
    SchedulerBinding.instance.addPostFrameCallback((_) => _controller.forward());

    _loadStats();
  }

  Future<void> _loadStats() async {
    final stored = await ScoreHistoryService.loadScores();
    if (stored.isNotEmpty) {
      setState(() {
        for (int i = 0; i < 4; i++) {
          String key = 'J${i + 1}';
          final entries = stored[key];
          if (entries != null && entries.isNotEmpty) {
            final scores = entries.map<int>((e) => (e['score'] ?? 0) as int).toList();
            final int total = scores.fold(0, (a, b) => a + b);
            final int best = scores.reduce((a, b) => a > b ? a : b);
            final int latest = scores.last;

            final freq = {0: 0, 5: 0, 10: 0, 15: 0, 25: 0, 50: 0, 100: 0, 150: 0, 200: 0, 250: 0};
            for (var s in scores) {
              if (freq.containsKey(s)) freq[s] = freq[s]! + 1;
            }

            final group = entries.last['group'] ?? '-';

            _playerStats[i]['score'] = latest;
            _playerStats[i]['total'] = total;
            _playerStats[i]['best'] = best;
            _playerStats[i]['games'] = scores.length;
            _playerStats[i]['group'] = group;
            _playerStats[i]['frequencies'] = freq;
            _playerStats[i]['history'] = entries;

            // Envoi des messages "Jx", "scores", "points", et "pointBonus"
            final ws = Provider.of<WebSocketService>(context, listen: false);
            final jsonMessage = {
              'Jx': i + 1,
              'scores': latest,
              'points': scores.last,
              'pointBonus': freq[scores.last] ?? 0,
            };
            ws.sendMessage(json.encode(jsonMessage));
          }
        }
      });
    }
  }

  void _resetPlayerStats(int index) {
    final ws = Provider.of<WebSocketService>(context, listen: false);

    setState(() {
      _playerStats[index] = {
        'score': 0,
        'total': 0,
        'best': 0,
        'games': 0,
        'group': '-',
        'frequencies': {0: 0, 5: 0, 10: 0, 15: 0, 25: 0, 50: 0, 100: 0, 150: 0, 200: 0, 250: 0},
        'history': <Map<String, dynamic>>[],
      };
    });

    final updatedScores = Map<int, int>.from(ws.scoresNotifier.value);
    updatedScores[index] = 0;
    ws.scoresNotifier.value = updatedScores;

    final updatedFreqs = Map<int, Map<int, int>>.from(ws.frequenciesNotifier.value);
    updatedFreqs[index] = {0: 0, 5: 0, 10: 0, 15: 0, 25: 0, 50: 0, 100: 0, 150: 0, 200: 0, 250: 0};
    ws.frequenciesNotifier.value = updatedFreqs;

    ws.resetCumulativeTimes();
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  /// Helper function to guess player count based on group format
  int _getPlayerCountFromGroup(String group) {
    // Exemples : CF1, CF2, CF3, CF4, CL2, LJ3, etc.
    final reg = RegExp(r'(\d)$');
    final match = reg.firstMatch(group);
    if (match != null) {
      final number = int.tryParse(match.group(1)!);
      if (number != null && number >= 1 && number <= 4) {
        return number;
      }
    }
    return 4; // fallback si on ne trouve pas
  }

  @override
  Widget build(BuildContext context) {
    final ws = Provider.of<WebSocketService>(context);
    final groupModel = Provider.of<GroupModel>(context, listen: false);
    final pseudos = ws.pseudonymsNotifier.value;

    // --- Gestion du nombre de joueurs actifs ---
    // Priorité : groupModel.selectedGroup > ws.groupNotifier.value > 4
    String groupRaw = groupModel.selectedGroup;
    if (groupRaw == '-' || groupRaw.isEmpty) {
      groupRaw = ws.groupNotifier.value;
    }
    int playerCount = _getPlayerCountFromGroup(groupRaw);

    return LayoutBuilder(
      builder: (context, constraints) {
        final double spacing = constraints.maxWidth * 0.02;

        return Scaffold(
          appBar: AppBar(
            title: const Row(
              children: [
                Icon(Icons.show_chart),
                SizedBox(width: 8),
                Text('Statistiques'),
              ],
            ),
            actions: [
              IconButton(
                icon: const Icon(Icons.open_in_new),
                tooltip: 'Ouvrir les cibles interactives',
                onPressed: () {
                  final currentOrigin = Uri.base.origin;
                  openInNewTab('$currentOrigin/ciblerie_interactive');
                },
              ),
            ],
          ),
          body: Padding(
            padding: EdgeInsets.all(spacing),
            child: FadeTransition(
              opacity: _fadeAnimation,
              child: ScaleTransition(
                scale: _scaleAnimation,
                child: ValueListenableBuilder<Map<int, int>>(
                  valueListenable: ws.scoresNotifier,
                  builder: (context, liveScores, _) {
                    if (liveScores.values.every((score) => score == 0)) {
                      for (int i = 0; i < 4; i++) {
                        if (_playerStats[i]['group'] != '-') {
                          _playerStats[i]['group'] = '-';
                        }
                      }
                    }

                    return ValueListenableBuilder<Map<int, Map<int, int>>>(
                      valueListenable: ws.frequenciesNotifier,
                      builder: (context, frequencies, _) {
                        return Row(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: List.generate(4, (index) {
                            final isActive = index < playerCount;

                            // Style/texte "désactivé" pour joueurs non utilisés,
                            // mais même design que cases actives
                            if (!isActive) {
                              return Expanded(
                                child: Container(
                                  margin: EdgeInsets.symmetric(horizontal: spacing / 2),
                                  padding: const EdgeInsets.all(16),
                                  decoration: BoxDecoration(
                                    color: greenGradient[index % greenGradient.length],
                                    borderRadius: BorderRadius.circular(16),
                                  ),
                                  child: Column(
                                    crossAxisAlignment: CrossAxisAlignment.start,
                                    children: [
                                      Text(
                                        pseudos[index] ?? 'J${index + 1}',
                                        style: const TextStyle(
                                          fontSize: 20,
                                          fontWeight: FontWeight.bold,
                                          color: Colors.black38,
                                        ),
                                      ),
                                      const SizedBox(height: 12),
                                      Text('Parties jouées : --', style: const TextStyle(color: Colors.black38)),
                                      Text('Groupe : --', style: const TextStyle(color: Colors.black38)),
                                      Text('Score : --', style: const TextStyle(color: Colors.black38)),
                                      Text('Totalisé : --', style: const TextStyle(color: Colors.black38)),
                                      Text('Meilleur score : --', style: const TextStyle(color: Colors.black38)),
                                      Text('Temps cumulé : --', style: const TextStyle(color: Colors.black38)),
                                      const SizedBox(height: 12),
                                      const Text('Tableau des fréquences :', style: TextStyle(fontWeight: FontWeight.bold, color: Colors.black38)),
                                      const SizedBox(height: 6),
                                      ...[0, 5, 10, 15, 25, 50, 100, 150, 200, 250].map((pt) => Text(
                                        '$pt points : --',
                                        style: const TextStyle(color: Colors.black38),
                                      )),
                                      const SizedBox(height: 12),
                                      const Text('Scores enregistrés :', style: TextStyle(fontWeight: FontWeight.bold, color: Colors.black38)),
                                      const SizedBox(height: 6),
                                      const Text("--", style: TextStyle(color: Colors.black38)),
                                    ],
                                  ),
                                ),
                              );
                            }

                            final player = _playerStats[index];
                            final List<Map<String, dynamic>> history =
                            List<Map<String, dynamic>>.from(player['history']);

                            final int currentScore = liveScores[index] ?? 0;
                            final int totalWithLive = history.fold<int>(
                              0,
                                  (sum, entry) => sum + ((entry['score'] ?? 0) as int),
                            );

                            final Map<int, int> liveFreq = frequencies[index] ?? {
                              0: 0,
                              5: 0,
                              10: 0,
                              15: 0,
                              25: 0,
                              50: 0,
                              100: 0,
                              150: 0,
                              200: 0,
                              250: 0,
                            };

                            // Groupe affiché live
                            final String groupToDisplay = currentScore > 0
                                ? groupModel.selectedGroup
                                : (player['group'] ?? '-');

                            return Expanded(
                              child: Container(
                                margin: EdgeInsets.symmetric(horizontal: spacing / 2),
                                padding: const EdgeInsets.all(16),
                                decoration: BoxDecoration(
                                  color: greenGradient[index % greenGradient.length],
                                  borderRadius: BorderRadius.circular(16),
                                ),
                                child: Column(
                                  crossAxisAlignment: CrossAxisAlignment.start,
                                  children: [
                                    Row(
                                      children: [
                                        Expanded(
                                          child: Text(
                                            pseudos[index] ?? 'J${index + 1}',
                                            style: const TextStyle(
                                              fontSize: 20,
                                              fontWeight: FontWeight.bold,
                                              color: Colors.black,
                                            ),
                                          ),
                                        ),
                                        ElevatedButton.icon(
                                          onPressed: () => _resetPlayerStats(index),
                                          style: ElevatedButton.styleFrom(
                                            backgroundColor: Colors.red,
                                            shape: RoundedRectangleBorder(
                                              borderRadius: BorderRadius.circular(12),
                                            ),
                                          ),
                                          icon: const Icon(Icons.refresh, color: Colors.black, size: 18),
                                          label: const Text(
                                            'Reset',
                                            style: TextStyle(
                                              color: Colors.black,
                                              fontWeight: FontWeight.bold,
                                            ),
                                          ),
                                        ),
                                      ],
                                    ),
                                    const SizedBox(height: 12),
                                    Text('Parties jouées : ${player['games']}', style: const TextStyle(fontSize: 14, color: Colors.black)),
                                    Text('Groupe : $groupToDisplay', style: const TextStyle(fontSize: 14, color: Colors.black)),
                                    Text('Score : $currentScore', style: const TextStyle(fontSize: 14, color: Colors.black)),
                                    Text('Totalisé : $totalWithLive', style: const TextStyle(fontSize: 14, color: Colors.black)),
                                    Text('Meilleur score : ${player['best']}', style: const TextStyle(fontSize: 14, color: Colors.black)),
                                    // Temps cumulé par joueur
                                    ValueListenableBuilder<Map<int, double>>(
                                      valueListenable: ws.cumulativeTimesNotifier,
                                      builder: (context, cumulativeTimes, _) {
                                        final time = cumulativeTimes[index] ?? 0.0;
                                        return Text(
                                          'Temps cumulé : ${formatTimeMinSec(time)}',
                                          style: const TextStyle(
                                              fontSize: 14, color: Colors.black
                                          ),
                                        );
                                      },
                                    ),
                                    const SizedBox(height: 12),
                                    const Text('Tableau des fréquences :', style: TextStyle(fontWeight: FontWeight.bold, color: Colors.black)),
                                    const SizedBox(height: 6),
                                    ...[0, 5, 10, 15, 25, 50, 100, 150, 200, 250].map((pt) => Text(
                                      '$pt points : ${liveFreq[pt] ?? 0}',
                                      style: const TextStyle(color: Colors.black),
                                    )),
                                    const SizedBox(height: 12),
                                    const Text('Scores enregistrés :', style: TextStyle(fontWeight: FontWeight.bold, color: Colors.black)),
                                    const SizedBox(height: 6),
                                    if (history.isEmpty)
                                      const Text(
                                        "Aucune donnée enregistrée.",
                                        style: TextStyle(fontSize: 14, color: Colors.black),
                                      ),
                                    ...history.map((entry) {
                                      // Affiche "CF1 : 126" (ou autre groupe réel)
                                      final String group = (entry['group'] ?? '-').toString().replaceAll(RegExp(r'^Groupe\s*'), '');
                                      final s = entry['score'] ?? 0;
                                      return Text('$group : $s', style: const TextStyle(color: Colors.black));
                                    }),
                                  ],
                                ),
                              ),
                            );
                          }),
                        );
                      },
                    );
                  },
                ),
              ),
            ),
          ),
        );
      },
    );
  }
}
