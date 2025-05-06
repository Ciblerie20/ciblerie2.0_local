import 'dart:convert';
import 'package:web_socket_channel/web_socket_channel.dart';

class WebSocketService {
  final String url;
  late WebSocketChannel _channel;

  WebSocketService(this.url) {
    _channel = WebSocketChannel.connect(Uri.parse(url));
  }

  /// Envoi d'un message au serveur WebSocket
  void sendMessage(String message) {
    _channel.sink.add(message);
  }

  /// Stream pour écouter les messages entrants
  Stream<dynamic> get messages => _channel.stream.map((event) {
    try {
      return jsonDecode(event);
    } catch (_) {
      return event; // Retour brut si le JSON est invalide
    }
  });

  /// Gestion des données spécifiques envoyées par l'ESP32
  Stream<Map<String, dynamic>> get esp32Data => messages.map((data) {
    if (data is Map<String, dynamic>) {
      // Vérifie si le message contient des informations pertinentes
      if (data.containsKey('playerIndex') &&
          data.containsKey('point') &&
          data.containsKey('score')) {
        return {
          'type': 'score',
          'playerIndex': data['playerIndex'],
          'point': data['point'],
          'score': data['score']
        };
      } else if (data.containsKey('type') && data['type'] == 'groupe') {
        return {
          'type': 'groupe',
          'value': data['value']
        };
      } else if (data.containsKey('type') && data['type'] == 'fin') {
        return {
          'type': 'fin',
          'message': data['message']
        };
      }
    }
    return {
      'type': 'unknown',
      'raw': data
    }; // Si le format est inconnu
  });

  /// Fermeture propre de la connexion WebSocket
  void closeConnection() {
    _channel.sink.close();
  }
}
