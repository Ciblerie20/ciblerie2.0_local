import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter_secure_storage/flutter_secure_storage.dart';

class NewRecordDialog extends StatefulWidget {
  final int playerId;
  final int score;
  final String pseudo;

  const NewRecordDialog({
    super.key,
    required this.playerId,
    required this.score,
    required this.pseudo,
  });

  @override
  State<NewRecordDialog> createState() => _NewRecordDialogState();
}

class _NewRecordDialogState extends State<NewRecordDialog> {
  final _storage = const FlutterSecureStorage();
  String? _errorMessage;

  @override
  Widget build(BuildContext context) {
    return AlertDialog(
      title: const Text('🎯 Nouveau Record !'),
      content: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Text('Pseudo: ${widget.pseudo}'),
          Text('Score: ${widget.score}'),
          if (_errorMessage != null)
            Text(
              _errorMessage!,
              style: const TextStyle(color: Colors.red),
            ),
        ],
      ),
      actions: [
        Center(
          child: ElevatedButton(
            onPressed: _handleValidation,
            child: const Text('Valider'),
          ),
        ),
      ],
    );
  }

  /// Gestion de la validation et de l'enregistrement du score
  Future<void> _handleValidation() async {
    try {
      final playerScores = await _loadPlayerScores();
      final pseudo = widget.pseudo;

      if (playerScores.containsKey(pseudo)) {
        final scores = List<int>.from(playerScores[pseudo]);
        final int bestScore = scores.isNotEmpty ? scores.reduce((a, b) => a > b ? a : b) : 0;

        if (widget.score <= bestScore) {
          setState(() {
            _errorMessage = 'Votre nouveau score n\'est pas un record pour ce pseudo.';
          });
          return;
        }

        scores.add(widget.score);
        playerScores[pseudo] = scores;
      } else {
        playerScores[pseudo] = [widget.score];
      }

      await _savePlayerScores(playerScores);

      if (!mounted) return;
      Navigator.of(context).pop(); // Ferme le dialog après validation
    } catch (e) {
      setState(() {
        _errorMessage = 'Une erreur est survenue lors de l\'enregistrement.';
      });
    }
  }

  /// Charge les scores enregistrés depuis le stockage sécurisé
  Future<Map<String, dynamic>> _loadPlayerScores() async {
    final json = await _storage.read(key: 'player_scores');
    if (json == null) {
      return {};
    }
    try {
      return Map<String, dynamic>.from(jsonDecode(json));
    } catch (e) {
      return {};
    }
  }

  /// Sauvegarde les scores dans le stockage sécurisé
  Future<void> _savePlayerScores(Map<String, dynamic> playerScores) async {
    final json = jsonEncode(playerScores);
    await _storage.write(key: 'player_scores', value: json);
  }
}
