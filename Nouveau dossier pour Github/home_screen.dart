import 'package:flutter/material.dart';

class HomeScreen extends StatelessWidget {
  @override
  final Key? key;

  const HomeScreen({this.key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Home'),
      ),
      body: Padding(
        padding: const EdgeInsets.all(24.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            const Icon(Icons.track_changes, size: 64.0, color: Colors.blueAccent),
            const SizedBox(height: 16),
            const Text(
              'Bienvenue dans Ciblerie Local',
              style: TextStyle(
                fontSize: 24.0,
                fontWeight: FontWeight.bold,
              ),
            ),
            const SizedBox(height: 32),
            ElevatedButton.icon(
              icon: const Icon(Icons.leaderboard),
              label: const Text('Voir les Scores'),
              onPressed: () => Navigator.pushNamed(context, '/scores'),
              style: ElevatedButton.styleFrom(
                minimumSize: const Size(double.infinity, 50),
              ),
            ),
            const SizedBox(height: 16),
            ElevatedButton.icon(
              icon: const Icon(Icons.bar_chart),
              label: const Text('Voir les Statistiques'),
              onPressed: () => Navigator.pushNamed(context, '/stats'),
              style: ElevatedButton.styleFrom(
                minimumSize: const Size(double.infinity, 50),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
