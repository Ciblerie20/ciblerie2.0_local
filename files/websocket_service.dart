import 'dart:convert';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';

// Importations pour utiliser le package web_socket_channel
import 'package:web_socket_channel/web_socket_channel.dart';
import 'package:web_socket_channel/io.dart';
import 'package:web_socket_channel/html.dart';

class WebSocketService with ChangeNotifier {
  WebSocketChannel? _channel;
  bool _isConnected = false;
  List<Map<String, dynamic>> playerData = [
    {'playerIndex': 0, 'score': 0},
    {'playerIndex': 1, 'score': 0},
    {'playerIndex': 2, 'score': 0},
    {'playerIndex': 3, 'score': 0},
  ];

  bool get isConnected => _isConnected;

  // Méthode de connexion qui s'adapte à la plateforme (Web ou non)
  void connect(String url) {
    Uri uri = Uri.parse(url);
    if (kIsWeb) {
      _channel = HtmlWebSocketChannel.connect(uri);
    } else {
      _channel = IOWebSocketChannel.connect(uri);
    }
    _isConnected = true;
    notifyListeners();

    print('✅ Connecté au WebSocket : $url');

    _channel!.stream.listen((message) {
      _handleIncomingMessage(message);
    }, onDone: () {
      print('❌ Connexion WebSocket terminée.');
      _disconnectInternal();
    }, onError: (error) {
      print('⚠️ Erreur WebSocket : $error');
      _disconnectInternal();
    });
  }

  // Méthode de déconnexion
  void disconnect() {
    if (_channel != null) {
      _channel!.sink.close();
      _disconnectInternal();
      print('❌ Déconnecté du WebSocket.');
    }
  }

  void _disconnectInternal() {
    _channel = null;
    _isConnected = false;
    notifyListeners();
  }

  // Traitement des messages entrants
  void _handleIncomingMessage(dynamic message) {
    try {
      final decodedMessage = json.decode(message);
      if (decodedMessage is Map<String, dynamic> &&
          decodedMessage.containsKey('playerIndex') &&
          decodedMessage.containsKey('score')) {
        final playerIndex = decodedMessage['playerIndex'] as int;
        final score = decodedMessage['score'] as int;
        if (playerIndex >= 0 && playerIndex < playerData.length) {
          playerData[playerIndex]['score'] = score;
          notifyListeners();
        }
      }
    } catch (error) {
      print('⚠️ Erreur lors du traitement du message WebSocket : $error');
    }
  }

  // Réinitialisation locale des scores (appelée lors de la connexion)
  void resetScores() {
    for (var player in playerData) {
      player['score'] = 0;
    }
    notifyListeners();
    print('🔄 Scores locaux réinitialisés.');
  }

  // Envoi d'une commande de réinitialisation au serveur ESP32
  void sendResetCommand() {
    if (_channel != null) {
      final resetMessage = json.encode({'type': 'reset'});
      _channel!.sink.add(resetMessage);
      print('📤 Commande de réinitialisation envoyée au serveur ESP32.');
    } else {
      print('⚠️ Impossible d\'envoyer la commande : WebSocket non connecté.');
    }
  }

  // Récupération des données de score pour une boîte spécifique
  Map<String, dynamic> getScoreForBox(int index) {
    if (index >= 0 && index < playerData.length) {
      return playerData[index];
    }
    return {'playerIndex': index, 'score': 0};
  }
}
