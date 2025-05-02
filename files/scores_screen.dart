import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../services/websocket_service.dart';
import '../services/score_history_service.dart';

/// Ce widget affiche l’icône wifi avec une animation de clignotement
/// si l’appareil n’est pas connecté.
class BlinkingWifiIcon extends StatefulWidget {
  final bool isConnected;
  const BlinkingWifiIcon({Key? key, required this.isConnected}) : super(key: key);

  @override
  _BlinkingWifiIconState createState() => _BlinkingWifiIconState();
}

class _BlinkingWifiIconState extends State<BlinkingWifiIcon>
    with SingleTickerProviderStateMixin {
  late AnimationController _controller;

  @override
  void initState() {
    super.initState();
    _controller = AnimationController(
      vsync: this,
      duration: Duration(milliseconds: 500),
    )..repeat(reverse: true);
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    if (widget.isConnected) {
      return Icon(Icons.wifi, size: 20);
    } else {
      return FadeTransition(
        opacity: _controller,
        child: Icon(Icons.wifi, size: 20),
      );
    }
  }
}

class ScoresScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final webSocketService = Provider.of<WebSocketService>(context);
    final historyService = Provider.of<ScoreHistoryService>(context);

    bool isConnected = webSocketService.isConnected;
    String connectionLabel = isConnected ? "Connecté" : "Se connecter";
    Color connectionColor = isConnected ? Colors.green : Colors.blue;

    double appBarHeight = AppBar().preferredSize.height; // Calcul de la hauteur de la barre d'app.

    return Scaffold(
      appBar: AppBar(
        title: Text('Scores'),
        actions: [
          if (!isConnected)
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 4.0),
              child: ElevatedButton.icon(
                style: ElevatedButton.styleFrom(
                  backgroundColor: Colors.grey[800],
                  minimumSize: Size(100, 36),
                  padding: EdgeInsets.symmetric(horizontal: 8),
                ),
                onPressed: () {
                  final scores = webSocketService.playerData
                      .map((player) => player['score'] as int)
                      .toList();
                  historyService.addScores(scores);
                  ScaffoldMessenger.of(context).showSnackBar(
                    SnackBar(content: Text('✅ Partie enregistrée')),
                  );
                },
                icon: Icon(Icons.save, size: 20),
                label: Text(
                  'Enregistrer',
                  style: TextStyle(fontSize: 12),
                ),
              ),
            ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 4.0),
            child: ElevatedButton.icon(
              style: ElevatedButton.styleFrom(
                backgroundColor: connectionColor,
                minimumSize: Size(100, 36),
                padding: EdgeInsets.symmetric(horizontal: 8),
              ),
              onPressed: () {
                webSocketService.resetScores();
                webSocketService.connect('ws://192.168.1.29:81');
              },
              icon: BlinkingWifiIcon(isConnected: isConnected),
              label: Text(
                connectionLabel,
                style: TextStyle(fontSize: 12),
              ),
            ),
          ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 4.0),
            child: ElevatedButton.icon(
              style: ElevatedButton.styleFrom(
                backgroundColor: Colors.orange,
                minimumSize: Size(100, 36),
                padding: EdgeInsets.symmetric(horizontal: 8),
              ),
              onPressed: () {
                webSocketService.sendResetCommand();
                webSocketService.disconnect();
              },
              icon: Icon(Icons.wifi_off, size: 20),
              label: Text(
                'Déconnexion',
                style: TextStyle(fontSize: 12),
              ),
            ),
          ),
        ],
      ),
      body: Padding(
        // Ajout de marges dynamiques en haut et en bas.
        padding: EdgeInsets.only(
          top: appBarHeight * 0.5, // Marge en haut proportionnelle.
          bottom: appBarHeight * 0.5, // Marge en bas proportionnelle.
        ),
        child: Column(
          children: [
            // Zone d'affichage des scores.
            Expanded(
              child: LayoutBuilder(
                builder: (context, constraints) {
                  if (isConnected) {
                    // Mode connecté : affichage en grille type Wrap (inchangé).
                    double caseHeight = constraints.maxHeight * 0.3;
                    double caseWidth = constraints.maxWidth * 0.4;
                    return Center(
                      child: Wrap(
                        spacing: 16.0,
                        runSpacing: 16.0,
                        children: List.generate(4, (index) {
                          final playerData = webSocketService.getScoreForBox(index);
                          final playerId = "J${index + 1}";
                          return Container(
                            width: caseWidth,
                            height: caseHeight,
                            decoration: BoxDecoration(
                              color: Colors.blue[100 + (index * 100)],
                              borderRadius: BorderRadius.circular(16.0),
                              boxShadow: [
                                BoxShadow(
                                  color: Colors.black.withOpacity(0.1),
                                  spreadRadius: 2,
                                  blurRadius: 5,
                                ),
                              ],
                            ),
                            child: Align(
                              alignment: Alignment.center,
                              child: Text(
                                "$playerId : ${playerData['score']}",
                                style: TextStyle(
                                  fontSize: caseHeight * 0.5, // Ajustement pour occuper 50% de la hauteur
                                  fontWeight: FontWeight.bold,
                                  color: Colors.blue[900],
                                ),
                                textAlign: TextAlign.center,
                              ),
                            ),
                          );
                        }),
                      ),
                    );
                  } else {
                    // Mode déconnecté : centrer le texte verticalement avec 90% d'occupation.
                    List<int> indices = List.generate(4, (index) => index);
                    indices.sort((a, b) {
                      int scoreA = webSocketService.getScoreForBox(a)['score'] as int;
                      int scoreB = webSocketService.getScoreForBox(b)['score'] as int;
                      return scoreB.compareTo(scoreA);
                    });
                    double rectHeight = constraints.maxHeight * 0.22;
                    double rectWidth = constraints.maxWidth * 0.95;
                    return SingleChildScrollView(
                      child: Column(
                        children: indices.map((index) {
                          final playerData = webSocketService.getScoreForBox(index);
                          final playerId = "J${index + 1}";
                          return Padding(
                            padding: const EdgeInsets.symmetric(
                                vertical: 8.0, horizontal: 16.0),
                            child: Container(
                              width: rectWidth,
                              height: rectHeight,
                              decoration: BoxDecoration(
                                color: Colors.blue[100 + (index * 100)],
                                borderRadius: BorderRadius.circular(16.0),
                                boxShadow: [
                                  BoxShadow(
                                    color: Colors.black.withOpacity(0.1),
                                    spreadRadius: 2,
                                    blurRadius: 5,
                                  ),
                                ],
                              ),
                              // Centrage vertical et horizontal du texte.
                              child: Align(
                                alignment: Alignment.center,
                                child: Text(
                                  "$playerId : ${playerData['score']}",
                                  style: TextStyle(
                                    fontSize: rectHeight * 0.5, // Ajustement pour occuper 50% de la hauteur
                                    fontWeight: FontWeight.bold,
                                    color: Colors.blue[900],
                                  ),
                                  textAlign: TextAlign.center,
                                ),
                              ),
                            ),
                          );
                        }).toList(),
                      ),
                    );
                  }
                },
              ),
            ),
          ],
        ),
      ),
    );
  }
}
