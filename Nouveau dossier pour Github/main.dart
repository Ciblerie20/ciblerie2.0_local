import 'package:flutter/material.dart';
import 'package:provider/provider.dart';

import 'screens/home_screen.dart';
import 'screens/scores_screen.dart';
import 'screens/stats_screen.dart';
import 'services/websocket_service.dart';
import 'services/score_history_service.dart'; // Ajout du service pour l’historique

void main() {
  runApp(
    MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => WebSocketService()),
        ChangeNotifierProvider(create: (_) => ScoreHistoryService()), // Ajout ici
      ],
      child: CiblerieLocalApp(),
    ),
  );
}

class CiblerieLocalApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Ciblerie Local',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      initialRoute: '/',
      routes: {
        '/': (context) => HomeScreen(),
        '/scores': (context) => ScoresScreen(),
        '/stats': (context) => StatsScreen(),
      },
    );
  }
}
