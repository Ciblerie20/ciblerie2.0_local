import 'dart:convert';
import 'package:flutter/foundation.dart';
import 'package:web_socket_channel/web_socket_channel.dart';
import 'package:web_socket_channel/io.dart';
import 'package:web_socket_channel/html.dart';

class WebSocketService with ChangeNotifier {
  WebSocketChannel? _channel;
  bool _isConnected = false;
  Map<String, dynamic> currentPlayerData = {
    'playerIndex': 0,
    'score': 0,
    'point': 0
  }; // Données actuelles du joueur
  bool _gameEnded = false; // Indicateur de fin de jeu

  bool get isConnected => _isConnected;
  bool get gameEnded => _gameEnded;

  // Liste des données des joueurs avec les "points" intégrés
  List<Map<String, dynamic>> playerData = [
    {'playerIndex': 0, 'score': 0, 'point': 0},
    {'playerIndex': 1, 'score': 0, 'point': 0},
    {'playerIndex': 2, 'score': 0, 'point': 0},
    {'playerIndex': 3, 'score': 0, 'point': 0},
  ];

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

    debugPrint('✅ Connecté au WebSocket : $url');

    _channel!.stream.listen((message) {
      _handleIncomingMessage(message);
    }, onDone: () {
      debugPrint('❌ Connexion WebSocket terminée.');
      _disconnectInternal();
    }, onError: (error) {
      debugPrint('⚠️ Erreur WebSocket : $error');
      _disconnectInternal();
    });
  }

  // Méthode de déconnexion
  void disconnect() {
    if (_channel != null) {
      _channel!.sink.close();
      _disconnectInternal();
      debugPrint('❌ Déconnecté du WebSocket.');
    }
  }

  void _disconnectInternal() {
    _channel = null;
    _isConnected = false;
    notifyListeners();
  }

  // Gestion des messages entrants
  void _handleIncomingMessage(dynamic message) {
    try {
      final decodedMessage = json.decode(message);

      // Gestion des données des joueurs
      if (decodedMessage is Map<String, dynamic> &&
          decodedMessage.containsKey('playerIndex') &&
          decodedMessage.containsKey('score') &&
          decodedMessage.containsKey('point')) {
        final playerIndex = decodedMessage['playerIndex'] as int;
        final score = decodedMessage['score'] as int;
        final point = decodedMessage['point'] as int;
        if (playerIndex >= 0 && playerIndex < playerData.length) {
          playerData[playerIndex]['score'] = score;
          playerData[playerIndex]['point'] = point;
          notifyListeners();
        }
      } 
      // Gestion du message de fin de partie
      else if (decodedMessage is Map<String, dynamic> &&
          decodedMessage.containsKey('type') &&
          decodedMessage['type'] == 'end' &&
          decodedMessage.containsKey('message') &&
          decodedMessage['message'] == 'FIN GAME') {
        _gameEnded = true;
        notifyListeners();
        debugPrint('🔚 Fin de la partie reçue via message JSON.');
      }
    } catch (error) {
      debugPrint('⚠️ Erreur lors du traitement du message WebSocket : $error');
    }
  }

  // Méthode pour réinitialiser les scores et points localement
  void resetScores() {
    for (var player in playerData) {
      player['score'] = 0;
      player['point'] = 0;
    }
    _gameEnded = false;
    notifyListeners();
    debugPrint('🔄 Scores locaux réinitialisés.');
  }

  // Méthode pour récupérer les données d’un joueur spécifique
  Map<String, dynamic> getScoreForBox(int index) {
    if (index >= 0 && index < playerData.length) {
      return playerData[index];
    }
    return {'playerIndex': index, 'score': 0, 'point': 0};
  }

  // Méthode pour envoyer une commande de réinitialisation au serveur ESP32
  void sendResetCommand() {
    if (_channel != null && _isConnected) {
      final resetMessage = json.encode({'type': 'reset'});
      _channel!.sink.add(resetMessage);
      debugPrint('📤 Commande de réinitialisation envoyée au serveur ESP32.');
    } else {
      debugPrint('⚠️ Impossible d\'envoyer la commande : WebSocket non connecté.');
    }
  }

  // Récupération des données actuelles du joueur
  Map<String, dynamic> getCurrentPlayerData() {
    return currentPlayerData;
  }
}