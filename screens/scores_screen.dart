import 'dart:async';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../services/websocket_service.dart';
import '../models/group_model.dart';
import '../models/game_ui_state.dart';
import 'classement_screen.dart';

class ScoresScreen extends StatefulWidget {
  const ScoresScreen({super.key});

  @override
  State<ScoresScreen> createState() => _ScoresScreenState();
}

class _ScoresScreenState extends State<ScoresScreen> with TickerProviderStateMixin {
  late AnimationController _controller;
  late AnimationController _overlayController;
  late Animation<double> _fadeAnimation;
  late Animation<double> _scaleAnimation;
  late AnimationController _blinkController;
  StreamSubscription<String>? _messageSubscription;

  bool showFinGameOverlay = false;
  bool hasNavigatedToClassement = false;

  @override
  void initState() {
    super.initState();

    _controller = AnimationController(duration: const Duration(milliseconds: 500), vsync: this);
    _fadeAnimation = Tween<double>(begin: 0, end: 1).animate(_controller);
    _scaleAnimation = Tween<double>(begin: 0.95, end: 1).animate(_controller);

    _overlayController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 500),
    );

    _blinkController = AnimationController(
      duration: const Duration(seconds: 1),
      vsync: this,
    );

    _controller.forward();

    WidgetsBinding.instance.addPostFrameCallback((_) {
      _updateBlinkingAnimation();
    });

    _setupWebSocketListener();
  }

  void _setupWebSocketListener() {
    final ws = Provider.of<WebSocketService>(context, listen: false);
    _messageSubscription = ws.messages.listen((message) {
      final data = json.decode(message) as Map<String, dynamic>;
      final type = data['type'] as String?;
      if (type != null) {
        _handleWebSocketMessage(type, message);
      }
    });
  }

  void _handleWebSocketMessage(String type, String message) {
    if (!mounted) return;
    final uiState = Provider.of<GameUIState>(context, listen: false);

    setState(() {
      if (type == 'game_status') {
        final data = json.decode(message) as Map<String, dynamic>;
        final wsMessage = data['message'] as String?;
        if (wsMessage == 'START') {
          uiState.updateNextPlayer(
            enabled: false,
            color: Colors.green,
            blinking: false,
          );
          uiState.updateNextTurn(
            enabled: false,
            color: Colors.green,
            blinking: false,
          );
          uiState.updateAttenteGroupe(
            color: Colors.orange,
            enabled: true,
            text: 'Partie en attente',
            blinking: true,
          );
        }
        // Activation du bouton "Tour suivant" sur réception du bon message
        if (wsMessage == 'NEXT_TURN') {
          uiState.updateNextTurn(
            enabled: true,
            color: Colors.orange,
            blinking: true,
          );
          uiState.updateNextPlayer(
            enabled: false,
            color: Colors.green,
            blinking: false,
          );
          uiState.updateAttenteGroupe(
            color: Colors.green,
            enabled: false,
            text: 'Partie lancée',
            blinking: false,
          );
        }
        if (wsMessage == 'NEXT_PLAYER') {
          uiState.updateNextPlayer(
            enabled: true,
            color: Colors.orange,
            blinking: true,
          );
          uiState.updateNextTurn(
            enabled: false,
            color: Colors.green,
            blinking: false,
          );
          uiState.updateAttenteGroupe(
            color: Colors.green,
            enabled: false,
            text: 'Partie lancée',
            blinking: false,
          );
        }
        if (wsMessage == 'FIN_GAME') {
          _showFinGameOverlay();
          uiState.updateNextPlayer(
            enabled: false,
            color: Colors.blue,
            blinking: false,
          );
          uiState.updateNextTurn(
            enabled: false,
            color: Colors.blue,
            blinking: false,
          );
          uiState.updateAttenteGroupe(
            color: const Color(0xFF7DBFF8),
            enabled: false,
            text: 'Attente groupe',
            blinking: false,
          );
        }
      }
    });

    WidgetsBinding.instance.addPostFrameCallback((_) {
      _updateBlinkingAnimation();
    });
  }

  void _updateBlinkingAnimation() {
    final uiState = Provider.of<GameUIState>(context, listen: false);
    final blinking = uiState.attenteGroupeBlinking || uiState.nextPlayerBlinking || uiState.nextTurnBlinking;
    if (blinking) {
      if (!_blinkController.isAnimating) {
        _blinkController.repeat(reverse: true);
      }
    } else {
      _blinkController.stop();
      _blinkController.value = 1.0;
    }
  }

  void _onStartGamePressed() {
    final ws = Provider.of<WebSocketService>(context, listen: false);
    ws.sendMessage(json.encode({'type': 'game_status', 'message': 'START_GAME'}));
    final uiState = Provider.of<GameUIState>(context, listen: false);
    uiState.updateAttenteGroupe(
      color: Colors.green,
      enabled: false,
      text: 'Partie lancée',
      blinking: false,
      disabledColor: Colors.green,
    );
    _updateBlinkingAnimation();
  }

  void _onNextPlayerPressed() {
    final ws = Provider.of<WebSocketService>(context, listen: false);
    ws.sendMessage(json.encode({'type': 'game_status', 'message': 'VALID_E'}));
    final uiState = Provider.of<GameUIState>(context, listen: false);
    uiState.updateNextPlayer(
      enabled: false,
      color: Colors.green,
      blinking: false,
    );
    uiState.updateNextTurn(
      enabled: false,
      color: Colors.green,
      blinking: false,
    );
    _updateBlinkingAnimation();
  }

  void _onNextTurnPressed() {
    final ws = Provider.of<WebSocketService>(context, listen: false);
    ws.sendMessage(json.encode({'type': 'game_status', 'message': 'VALID_E'}));
    final uiState = Provider.of<GameUIState>(context, listen: false);
    uiState.updateNextPlayer(
      enabled: false,
      color: Colors.green,
      blinking: false,
    );
    uiState.updateNextTurn(
      enabled: false,
      color: Colors.green,
      blinking: false,
    );
    _updateBlinkingAnimation();
  }

  void _showFinGameOverlay() async {
    if (!mounted) return;

    setState(() => showFinGameOverlay = true);
    _overlayController.forward();

    await Future.delayed(const Duration(seconds: 5));
    if (!mounted) return;
    _overlayController.reverse();

    await Future.delayed(const Duration(milliseconds: 500));
    if (mounted && !hasNavigatedToClassement) {
      hasNavigatedToClassement = true;

      final scoresMap = Provider.of<WebSocketService>(context, listen: false).scoresNotifier.value;
      final finalScores = List.generate(4, (i) => scoresMap[i] ?? 0);

      setState(() => showFinGameOverlay = false);

      Provider.of<WebSocketService>(context, listen: false).showSaveButton.value = true;

      Navigator.of(context).pushReplacement(
        MaterialPageRoute(
          builder: (_) => ClassementScreen(finalScores: finalScores),
        ),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    final ws = Provider.of<WebSocketService>(context);
    final groupModel = Provider.of<GroupModel>(context);
    final uiState = Provider.of<GameUIState>(context);

    _updateBlinkingAnimation();

    return Scaffold(
      appBar: AppBar(
        backgroundColor: Colors.black,
        title: const Row(
          children: [
            Icon(Icons.bar_chart, color: Colors.white),
            SizedBox(width: 8),
            Text('Scores', style: TextStyle(color: Colors.white)),
          ],
        ),
        actions: [
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 8.0),
            child: ElevatedButton(
              onPressed: null,
              style: ElevatedButton.styleFrom(
                backgroundColor: const Color(0xFF7DBFF8),
                disabledBackgroundColor: const Color(0xFF7DBFF8),
                elevation: 0,
                shape: RoundedRectangleBorder(
                  borderRadius: BorderRadius.circular(20),
                ),
              ),
              child: Text(
                'Groupe ${groupModel.selectedGroup}',
                style: const TextStyle(
                  fontWeight: FontWeight.bold,
                  color: Colors.black,
                ),
              ),
            ),
          ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 8.0),
            child: FadeTransition(
              opacity: uiState.attenteGroupeBlinking
                  ? Tween<double>(begin: 1.0, end: 0.0).animate(_blinkController)
                  : const AlwaysStoppedAnimation(1.0),
              child: ElevatedButton(
                onPressed: uiState.attenteGroupeEnabled ? _onStartGamePressed : null,
                style: ElevatedButton.styleFrom(
                  backgroundColor: uiState.attenteGroupeColor,
                  disabledBackgroundColor: uiState.attenteGroupeDisabledColor,
                  elevation: 0,
                  shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(20),
                  ),
                ),
                child: Text(
                  uiState.attenteGroupeText,
                  style: const TextStyle(
                    fontWeight: FontWeight.bold,
                    color: Colors.black,
                  ),
                ),
              ),
            ),
          ),
          if (uiState.attenteGroupeText != "Attente groupe") ...[
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8.0),
              child: FadeTransition(
                opacity: uiState.nextPlayerBlinking
                    ? Tween<double>(begin: 1.0, end: 0.0).animate(_blinkController)
                    : const AlwaysStoppedAnimation(1.0),
                child: ElevatedButton(
                  onPressed: uiState.nextPlayerEnabled ? _onNextPlayerPressed : null,
                  style: ElevatedButton.styleFrom(
                    backgroundColor: uiState.nextPlayerColor,
                    disabledBackgroundColor: Colors.blue,
                    elevation: 0,
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(20),
                    ),
                  ),
                  child: const Text(
                    'Joueur suivant',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                ),
              ),
            ),
            Padding(
              padding: const EdgeInsets.symmetric(horizontal: 8.0),
              child: FadeTransition(
                opacity: uiState.nextTurnBlinking
                    ? Tween<double>(begin: 1.0, end: 0.0).animate(_blinkController)
                    : const AlwaysStoppedAnimation(1.0),
                child: ElevatedButton(
                  onPressed: uiState.nextTurnEnabled ? _onNextTurnPressed : null,
                  style: ElevatedButton.styleFrom(
                    backgroundColor: uiState.nextTurnColor,
                    disabledBackgroundColor: Colors.blue,
                    elevation: 0,
                    shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(20),
                    ),
                  ),
                  child: const Text(
                    'Tour suivant',
                    style: TextStyle(
                      fontWeight: FontWeight.bold,
                      color: Colors.black,
                    ),
                  ),
                ),
              ),
            ),
          ],
        ],
      ),
      body: SafeArea(
        child: LayoutBuilder(
          builder: (context, constraints) {
            final double spacing = constraints.maxWidth * 0.02;
            final double usableHeight = constraints.maxHeight - spacing * 3;
            final double cardHeight = (usableHeight / 2) - spacing;
            final double cardWidth = (constraints.maxWidth - spacing * 3) / 2;
            final double fontSize = cardHeight * 0.4;

            return FadeTransition(
              opacity: _fadeAnimation,
              child: ScaleTransition(
                scale: _scaleAnimation,
                child: Stack(
                  alignment: Alignment.center,
                  children: [
                    ValueListenableBuilder<Map<int, String>>(
                      valueListenable: ws.pseudonymsNotifier,
                      builder: (context, pseudos, _) {
                        return ValueListenableBuilder<Map<int, int>>(
                          valueListenable: ws.scoresNotifier,
                          builder: (context, scores, _) {
                            return Column(
                              mainAxisAlignment: MainAxisAlignment.center,
                              children: [
                                Row(
                                  mainAxisAlignment: MainAxisAlignment.center,
                                  children: [
                                    _buildScoreCard(
                                      '${pseudos[0] ?? "J1"} : ${scores[0] ?? 0}',
                                      cardWidth,
                                      cardHeight,
                                      fontSize,
                                      const LinearGradient(
                                        colors: [Color(0xFFAEDCFA), Color(0xFF91C9F9)],
                                      ),
                                    ),
                                    SizedBox(width: spacing),
                                    _buildScoreCard(
                                      '${pseudos[1] ?? "J2"} : ${scores[1] ?? 0}',
                                      cardWidth,
                                      cardHeight,
                                      fontSize,
                                      const LinearGradient(
                                        colors: [Color(0xFF91C9F9), Color(0xFF6AB8F7)],
                                      ),
                                    ),
                                  ],
                                ),
                                SizedBox(height: spacing),
                                Row(
                                  mainAxisAlignment: MainAxisAlignment.center,
                                  children: [
                                    _buildScoreCard(
                                      '${pseudos[2] ?? "J3"} : ${scores[2] ?? 0}',
                                      cardWidth,
                                      cardHeight,
                                      fontSize,
                                      const LinearGradient(
                                        colors: [Color(0xFF6AB8F7), Color(0xFF429CF2)],
                                      ),
                                    ),
                                    SizedBox(width: spacing),
                                    _buildScoreCard(
                                      '${pseudos[3] ?? "J4"} : ${scores[3] ?? 0}',
                                      cardWidth,
                                      cardHeight,
                                      fontSize,
                                      const LinearGradient(
                                        colors: [Color(0xFF429CF2), Color(0xFF1E88E5)],
                                      ),
                                    ),
                                  ],
                                ),
                              ],
                            );
                          },
                        );
                      },
                    ),
                    if (showFinGameOverlay)
                      FadeTransition(
                        opacity: _overlayController,
                        child: Container(
                          width: constraints.maxWidth * 0.75,
                          height: constraints.maxHeight * 0.75,
                          decoration: BoxDecoration(
                            gradient: const LinearGradient(
                              colors: [Color(0xFFFFE0B2), Color(0xFFFFB74D)],
                            ),
                            borderRadius: BorderRadius.circular(20),
                          ),
                          alignment: Alignment.center,
                          child: FittedBox(
                            fit: BoxFit.scaleDown,
                            child: Text(
                              'FIN GAME',
                              style: TextStyle(
                                fontSize: constraints.maxHeight * 0.15,
                                fontWeight: FontWeight.bold,
                                color: Colors.black,
                              ),
                            ),
                          ),
                        ),
                      ),
                  ],
                ),
              ),
            );
          },
        ),
      ),
    );
  }

  Widget _buildScoreCard(String text, double width, double height, double fontSize, LinearGradient gradient) {
    return Container(
      width: width,
      height: height,
      decoration: BoxDecoration(
        gradient: gradient,
        borderRadius: BorderRadius.circular(16),
      ),
      alignment: Alignment.center,
      child: Text(
        text,
        style: TextStyle(
          fontSize: fontSize,
          fontWeight: FontWeight.bold,
          color: Colors.white,
        ),
        textAlign: TextAlign.center,
      ),
    );
  }

  @override
  void dispose() {
    _controller.dispose();
    _overlayController.dispose();
    _blinkController.dispose();
    _messageSubscription?.cancel();
    super.dispose();
  }
}
