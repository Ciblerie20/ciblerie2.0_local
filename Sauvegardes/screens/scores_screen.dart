import 'package:flutter/material.dart';
import '../services/websocket_service.dart'; // Import corrigé pour le service WebSocket

class ScoresScreen extends StatefulWidget {
  const ScoresScreen({Key? key}) : super(key: key);

  @override
  _ScoresScreenState createState() => _ScoresScreenState();
}

class _ScoresScreenState extends State<ScoresScreen> {
  late final WebSocketService _webSocketService;
  List<Map<String, dynamic>> scores = [
    {'joueur': 'J1', 'score': 0, 'color': Colors.blue.shade200},
    {'joueur': 'J2', 'score': 0, 'color': Colors.blue.shade300},
    {'joueur': 'J3', 'score': 0, 'color': Colors.blue.shade400},
    {'joueur': 'J4', 'score': 0, 'color': Colors.blue.shade500},
  ];

  @override
  void initState() {
    super.initState();
    // Initialisation du service WebSocket
    _webSocketService = WebSocketService('ws://192.168.1.1:81'); // Remplacez par l'URL de votre serveur WebSocket

    // Écoute des messages WebSocket
    _webSocketService.messages.listen((message) {
      if (message is Map<String, dynamic> && message.containsKey('playerIndex')) {
        setState(() {
          int playerIndex = message['playerIndex'];
          if (playerIndex >= 0 && playerIndex < scores.length) {
            scores[playerIndex]['score'] = message['score'];
          }
        });
      }
    });
  }

  @override
  void dispose() {
    _webSocketService.closeConnection();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white,
        elevation: 0,
        leading: IconButton(
          icon: const Icon(Icons.arrow_back, color: Colors.black),
          onPressed: () {
            Navigator.pop(context);
          },
        ),
        title: const Text(
          'Scores',
          style: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.bold,
            color: Colors.black,
          ),
        ),
        centerTitle: true,
      ),
      body: Padding(
        padding: const EdgeInsets.all(16.0),
        child: LayoutBuilder(
          builder: (context, constraints) {
            final double itemHeight = (constraints.maxHeight - 48) / 2;
            final double itemWidth = (constraints.maxWidth - 32) / 2;

            return GridView.builder(
              gridDelegate: SliverGridDelegateWithFixedCrossAxisCount(
                crossAxisCount: 2,
                crossAxisSpacing: 16,
                mainAxisSpacing: 16,
                childAspectRatio: itemWidth / itemHeight,
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
                    borderRadius: BorderRadius.circular(20),
                    boxShadow: [
                      BoxShadow(
                        color: Colors.black.withOpacity(0.1),
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
                            fontSize: 72,
                            fontWeight: FontWeight.bold,
                            color: Colors.white,
                          ),
                        ),
                        const SizedBox(height: 12),
                        Text(
                          '$score',
                          style: const TextStyle(
                            fontSize: 72,
                            fontWeight: FontWeight.bold,
                            color: Colors.white,
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