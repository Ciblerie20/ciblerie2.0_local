import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';
import 'classement_screen.dart';
import 'scores_screen.dart';
import 'stats_screen.dart';
import 'websocket_service.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({Key? key}) : super(key: key);

  @override
  State<HomeScreen> createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  String? ipAddress;
  WebSocketService? webSocketService;
  BluetoothConnection? bluetoothConnection;

  @override
  void initState() {
    super.initState();
    _initializeBluetoothConnection();
  }

  void _initializeBluetoothConnection() async {
    try {
      // Tentative de connexion Bluetooth à l'ESP32
      bluetoothConnection = await BluetoothConnection.toAddress("ESP32_BT_Config");
      bluetoothConnection!.input!.listen((data) {
        final message = utf8.decode(data);
        if (message.contains("📶 Adresse IP :")) {
          setState(() {
            ipAddress = message.split(":").last.trim();
            webSocketService = WebSocketService("ws://$ipAddress:81");
          });
          bluetoothConnection!.finish(); // Terminer la connexion Bluetooth
        }
      }).onDone(() {
        debugPrint("Connexion Bluetooth terminée.");
      });
    } catch (error) {
      debugPrint("Erreur lors de la connexion Bluetooth : $error");
    }
  }

  @override
  void dispose() {
    bluetoothConnection?.dispose();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.white, // Couleur de fond de l'AppBar
        elevation: 0, // Supprime l'ombre de l'AppBar
        title: const Text(
          'Bienvenue sur la Cible Électronique 2.0', // Texte ajouté dans l'AppBar
          style: TextStyle(
            fontSize: 18,
            fontWeight: FontWeight.bold,
            color: Colors.black, // Couleur du texte de l'AppBar
          ),
        ),
        actions: [
          _buildStyledButton(
            label: 'Scores',
            emoji: '📊', // Émoticône pour les scores
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ScoresScreen()),
              );
            },
          ),
          _buildStyledButton(
            label: 'Classement',
            emoji: '🏆', // Émoticône pour le classement
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const ClassementScreen()),
              );
            },
          ),
          _buildStyledButton(
            label: 'Stats',
            emoji: '📈', // Émoticône pour les statistiques
            onPressed: () {
              Navigator.push(
                context,
                MaterialPageRoute(builder: (context) => const StatsScreen()),
              );
            },
          ),
        ],
      ),
      body: Center(
        child: ipAddress == null
            ? const CircularProgressIndicator() // Indicateur de chargement en attente de l'adresse IP
            : Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: [
                  Text(
                    "Adresse IP récupérée : $ipAddress",
                    style: const TextStyle(fontSize: 16, fontWeight: FontWeight.bold),
                  ),
                  const SizedBox(height: 20),
                  ElevatedButton(
                    onPressed: () {
                      if (webSocketService != null) {
                        // Exemple d'interaction avec le WebSocketService
                        webSocketService!.sendMessage("Test message depuis l'application !");
                      }
                    },
                    child: const Text("Tester WebSocket"),
                  ),
                ],
              ),
      ),
    );
  }

  Widget _buildStyledButton({
    required String label,
    required String emoji,
    required VoidCallback onPressed,
  }) {
    return Padding(
      padding: const EdgeInsets.symmetric(horizontal: 5, vertical: 8), // Espacement autour des boutons
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
          foregroundColor: Colors.white, // Couleur du texte (blanc)
          backgroundColor: Colors.blue, // Couleur de fond (bleu)
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20), // Coins arrondis
          ),
          padding: const EdgeInsets.symmetric(horizontal: 15, vertical: 10), // Taille du bouton
        ),
        child: Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            Text(
              emoji,
              style: const TextStyle(fontSize: 16), // Taille de l'emoji
            ),
            const SizedBox(width: 5), // Espacement entre l'emoji et le texte
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