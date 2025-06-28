import 'package:flutter/material.dart';

class GameUIState extends ChangeNotifier {
  // ATTENTE GROUPE
  bool attenteGroupeEnabled = false;
  Color attenteGroupeColor = const Color(0xFF7DBFF8);
  String attenteGroupeText = 'Partie en attente';
  bool attenteGroupeBlinking = false;
  Color attenteGroupeDisabledColor = const Color(0xFF7DBFF8);

  // JOUEUR SUIVANT
  bool nextPlayerEnabled = false;
  Color nextPlayerColor = Colors.blue;
  bool nextPlayerBlinking = false;

  // TOUR SUIVANT
  bool nextTurnEnabled = false;
  Color nextTurnColor = Colors.blue;
  bool nextTurnBlinking = false;

  // ----
  void updateAttenteGroupe({
    required bool enabled,
    required Color color,
    required String text,
    required bool blinking,
    Color? disabledColor,
  }) {
    attenteGroupeEnabled = enabled;
    attenteGroupeColor = color;
    attenteGroupeText = text;
    attenteGroupeBlinking = blinking;
    attenteGroupeDisabledColor = disabledColor ?? color;
    notifyListeners();
  }

  void updateNextPlayer({
    required bool enabled,
    required Color color,
    required bool blinking,
  }) {
    nextPlayerEnabled = enabled;
    nextPlayerColor = color;
    nextPlayerBlinking = blinking;
    notifyListeners();
  }

  void updateNextTurn({
    required bool enabled,
    required Color color,
    required bool blinking,
  }) {
    nextTurnEnabled = enabled;
    nextTurnColor = color;
    nextTurnBlinking = blinking;
    notifyListeners();
  }

  void reset() {
    attenteGroupeEnabled = false;
    attenteGroupeColor = const Color(0xFF7DBFF8);
    attenteGroupeText = 'Partie en attente';
    attenteGroupeBlinking = false;
    attenteGroupeDisabledColor = const Color(0xFF7DBFF8);
    nextPlayerEnabled = false;
    nextPlayerColor = Colors.blue;
    nextPlayerBlinking = false;
    nextTurnEnabled = false;
    nextTurnColor = Colors.blue;
    nextTurnBlinking = false;
    notifyListeners();
  }
}
