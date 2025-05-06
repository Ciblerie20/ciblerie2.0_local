import 'package:flutter/material.dart';
import 'classement_screen.dart';
import 'scores_screen.dart';
import 'stats_screen.dart';
import 'package:ciblerie_local/services/websocket_service.dart'; // Import pour le service WebSocket

class HomeScreen extends StatelessWidget {
  const HomeScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white,
        elevation: 0,
        title: const Text(
          'Bienvenue sur la Cible Électronique 2.0',
          style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold, color: Colors.black),
        ),
        actions: [
          _buildStyledButton(
            label: 'Scores',
            emoji: '📊',
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ScoresScreen()),
              );
            },
          ),
          _buildStyledButton(
            label: 'Classement',
            emoji: '🏆',
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ClassementScreen()),
              );
            },
          ),
          _buildStyledButton(
            label: 'Stats',
            emoji: '📈',
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const StatsScreen()),
              );
            },
          ),
          const ConnectionButton(), // Bouton dynamique pour la connexion
        ],
      ),
      body: Container(),
    );
  }

  Widget _buildStyledButton({
    required String label,
    required String emoji,
    required VoidCallback onPressed,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 5, vertical: 8),
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
          foregroundColor: Colors.white,
          backgroundColor: Colors.blue,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20),
          ),
          padding: const EdgeInsets.symmetric(horizontal: 15, vertical: 10),
        ),
        child: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            Text(
              emoji,
              style: const TextStyle(fontSize: 16),
            ),
            const SizedBox(width: 5),
            Text(
              label,
              style: const TextStyle(fontSize: 14, fontWeight: FontWeight.bold),
            ),
          ],
        ),
      ),
    );
  }
}

// Bouton de connexion/déconnexion dynamique
class ConnectionButton extends StatefulWidget {
  const ConnectionButton({Key? key}) : super(key: key);

  @override
  State<ConnectionButton> createState() => _ConnectionButtonState();
}

class _ConnectionButtonState extends State<ConnectionButton> with SingleTickerProviderStateMixin {
  bool isConnected = false; // État de la connexion
  bool isConnecting = false; // Indique si la connexion est en cours
  late WebSocketService webSocketService; // Instance du service WebSocket
  late AnimationController _controller; // Contrôleur pour l'animation

  @override
  void initState() {
    super.initState();
    webSocketService = WebSocketService("ws://192.168.0.1:81"); // Remplacez par l'URL correcte
    _controller = AnimationController(
      duration: const Duration(seconds: 1),
      vsync: this,
    )..repeat(reverse: true); // Animation de clignotement
  }

  @override
  void dispose() {
    _controller.dispose();
    super.dispose();
  }

  Future<void> connectToLocalNetwork() async {
    setState(() {
      isConnecting = true;
    });
    print("Tentative de connexion au réseau local...");

    try {
      await Future.delayed(const Duration(seconds: 2)); // Simulation d'une tentative
      webSocketService.sendMessage("Test de connexion");
      setState(() {
        isConnected = true;
        isConnecting = false;
      });
      print("Connexion réussie !");
    } catch (e) {
      setState(() {
        isConnecting = false;
      });
      print("Erreur lors de la connexion : $e");
    }
  }

  void disconnectFromLocalNetwork() {
    setState(() {
      isConnected = false;
      isConnecting = false;
    });
    print("Déconnexion réussie !");
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      children: [
        // Bouton principal (Se connecter, Connexion en cours, Connecté)
        Padding(
          padding: const EdgeInsets.symmetric(horizontal: 4.0),
          child: ElevatedButton.icon(
            style: ElevatedButton.styleFrom(
              backgroundColor: isConnected
                  ? Colors.green
                  : (isConnecting ? Colors.blue : Colors.orange),
              minimumSize: const Size(120, 36),
            ),
            onPressed: isConnected || isConnecting
                ? null
                : connectToLocalNetwork, // Action dynamique
            icon: isConnecting
                ? FadeTransition(
              opacity: _controller,
              child: const Icon(Icons.wifi, color: Colors.white),
            )
                : Icon(
              Icons.wifi,
              color: Colors.white,
            ),
            label: Text(
              isConnected
                  ? "Connecté"
                  : (isConnecting ? "Connexion..." : "Se connecter"),
              style: const TextStyle(color: Colors.white),
            ),
          ),
        ),
        // Bouton de déconnexion
        if (isConnected)
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 4.0),
            child: ElevatedButton.icon(
              style: ElevatedButton.styleFrom(
                backgroundColor: Colors.orange,
                minimumSize: const Size(120, 36),
              ),
              onPressed: disconnectFromLocalNetwork, // Action de déconnexion
              icon: const Icon(
                Icons.wifi_off,
                color: Colors.white,
              ),
              label: const Text(
                "Déconnexion",
                style: TextStyle(color: Colors.white),
              ),
            ),
          ),
      ],
    );
  }
}