import 'dart:convert';
import 'package:web_socket_channel/web_socket_channel.dart';

class WebSocketService {
  final String url;
  late WebSocketChannel _channel;

  WebSocketService(this.url) {
    _channel = WebSocketChannel.connect(Uri.parse(url));
  }

  void sendMessage(String message) {
    _channel.sink.add(message);
  }

  Stream<dynamic> get messages => _channel.stream.map((event) {
        try {
          return jsonDecode(event);
        } catch (_) {
          return event;
        }
      });

  void closeConnection() {
    _channel.sink.close();
  }
}