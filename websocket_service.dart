import 'dart:async';
import 'dart:convert';
import 'package:web_socket_channel/web_socket_channel.dart';
import 'package:web_socket_channel/status.dart' as status;

class WebSocketService {
  final String url;
  late WebSocketChannel _channel;
  bool _isConnected = false;
  final StreamController<String> _connectionStatusController = StreamController<String>.broadcast();

  WebSocketService(this.url) {
    _initializeWebSocket();
  }

  /// Initialise la connexion WebSocket
  void _initializeWebSocket() {
    try {
      _channel = WebSocketChannel.connect(Uri.parse(url));
      _isConnected = true;
      _notifyConnectionStatus("Connected to WebSocket at $url");

      // Écouter les erreurs et la fermeture de la connexion
      _channel.stream.listen(
        (event) {},
        onError: (error) {
          _isConnected = false;
          _notifyConnectionStatus("WebSocket error: $error");
          _attemptReconnect();
        },
        onDone: () {
          _isConnected = false;
          _notifyConnectionStatus("WebSocket connection closed");
          _attemptReconnect();
        },
      );
    } catch (e) {
      _isConnected = false;
      _notifyConnectionStatus("WebSocket initialization error: $e");
      _attemptReconnect();
    }
  }

  /// Envoi d'un message au serveur WebSocket
  void sendMessage(String message) {
    try {
      if (_isConnected) {
        _channel.sink.add(message);
      } else {
        throw Exception("WebSocket is not connected");
      }
    } catch (error) {
      print("Erreur lors de l'envoi du message WebSocket : $error");
    }
  }

  /// Réception des messages entrants
  Stream<dynamic> get messages => _channel.stream.map((event) {
        try {
          return jsonDecode(event);
        } catch (_) {
          return event; // Retour brut si le JSON est invalide
        }
      });

  /// Réinitialiser la connexion WebSocket avec une nouvelle URL
  void resetConnection(String newUrl) {
    try {
      closeConnection();
      url = newUrl;
      _initializeWebSocket();
      print("Connexion WebSocket réinitialisée avec la nouvelle URL : $url");
    } catch (error) {
      print("Erreur lors de la réinitialisation de la connexion WebSocket : $error");
    }
  }

  /// Fermeture propre de la connexion WebSocket
  void closeConnection() {
    try {
      _channel.sink.close(status.normalClosure);
      _isConnected = false;
      _notifyConnectionStatus("WebSocket connection closed manually");
    } catch (error) {
      print("Erreur lors de la fermeture de la connexion WebSocket : $error");
    }
  }

  /// Tentative de reconnexion en cas de déconnexion
  void _attemptReconnect() {
    const reconnectDelay = Duration(seconds: 5);
    print("Tentative de reconnexion dans ${reconnectDelay.inSeconds} secondes...");
    Future.delayed(reconnectDelay, () {
      if (!_isConnected) {
        print("Tentative de reconnexion au WebSocket...");
        _initializeWebSocket();
      }
    });
  }

  /// Flux pour surveiller l'état de la connexion
  Stream<String> get connectionStatus => _connectionStatusController.stream;

  /// Notifie l'état de la connexion
  void _notifyConnectionStatus(String status) {
    _connectionStatusController.add(status);
    print("État de la connexion WebSocket : $status");
  }

  /// Libérer les ressources
  void dispose() {
    _connectionStatusController.close();
    closeConnection();
  }
}