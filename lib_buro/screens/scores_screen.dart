import 'dart:async';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:flutter/scheduler.dart';
import 'package:provider/provider.dart';
import '../services/websocket_service.dart';
import 'classement_screen.dart';

class ScoresScreen extends StatefulWidget {
  const ScoresScreen({super.key});

  @override
  State<ScoresScreen> createState() => _ScoresScreenState();
}

class _ScoresScreenState extends State<ScoresScreen> with TickerProviderStateMixin {
  late AnimationController _controller;
  late AnimationController _overlayController;
  late AnimationController _playerOverlayController;
  late AnimationController _tourOverlayController;
  late Animation<double> _fadeAnimation;
  late Animation<double> _scaleAnimation;

  bool showFinGameOverlay = false;
  bool showPlayerOverlay = false;
  bool showTourOverlay = false;
  bool hasNavigatedToClassement = false;
  StreamSubscription<String>? _messageSubscription;

  bool isGameStarted = false; // Variable pour savoir si la partie est lancée
  int currentPlayer = 0; // Joueur actuel
  int currentTour = 1; // Tour actuel

  @override
  void initState() {
    super.initState();

    // Animation initiale
    _controller = AnimationController(
      duration: const Duration(milliseconds: 500),
      vsync: this,
    );
    _fadeAnimation = Tween<double>(begin: 0, end: 1).animate(_controller);
    _scaleAnimation = Tween<double>(begin: 0.95, end: 1).animate(_controller);

    // Lancer l'animation au rendu de l'écran
    SchedulerBinding.instance.addPostFrameCallback((_) => _controller.forward());

    // Contrôleur de l'overlay général
    _overlayController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 500),
    );

    // Contrôleurs spécifiques pour les overlays de "Joueur suivant" et "Tour suivant"
    _playerOverlayController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 500),
    );
    _tourOverlayController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 500),
    );

    // Abonnement aux messages WebSocket
    final webSocketService = Provider.of<WebSocketService>(context, listen: false);
    _messageSubscription = webSocketService.messages.listen(_handleWebSocketMessage);
  }

  void _handleWebSocketMessage(String message) {
    try {
      final data = json.decode(message);
      if (data is Map<String, dynamic>) {
        if (data['type'] == 'fin') {
          _showFinGameOverlay();
        } else if (data['type'] == 'start') {
          Provider.of<WebSocketService>(context, listen: false).resetAllScores();
        } else if (data['event'] == 'JOUEUR_SUIVANT') {
          currentPlayer = data['player'];
          _showPlayerOverlay();
        } else if (data['event'] == 'TOUR_SUIVANT') {
          currentTour = data['tour'];
          _showTourOverlay();
        }
      }
    } catch (e) {
      debugPrint('Erreur WebSocket : $e');
    }
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

      Navigator.of(context).pushReplacement(
        MaterialPageRoute(
          builder: (_) => ClassementScreen(finalScores: finalScores),
        ),
      );
    }
  }

  void _showPlayerOverlay() {
    if (!mounted) return;
    setState(() => showPlayerOverlay = true);
    _playerOverlayController.forward();
  }

  void _hidePlayerOverlay() {
    if (!mounted) return;
    _playerOverlayController.reverse();
    Future.delayed(const Duration(milliseconds: 500), () {
      if (mounted) {
        setState(() => showPlayerOverlay = false);
      }
    });
  }

  void _showTourOverlay() {
    if (!mounted) return;
    setState(() => showTourOverlay = true);
    _tourOverlayController.forward();
  }

  void _hideTourOverlay() {
    if (!mounted) return;
    _tourOverlayController.reverse();
    Future.delayed(const Duration(milliseconds: 500), () {
      if (mounted) {
        setState(() => showTourOverlay = false);
      }
    });
  }

  void _sendNextPlayer() {
    final webSocketService = Provider.of<WebSocketService>(context, listen: false);
    currentPlayer = (currentPlayer + 1) % 4;
    webSocketService.sendMessage(json.encode({
      'type': 'game_event',
      'event': 'JOUEUR_SUIVANT',
      'player': currentPlayer,
    }));

    _showPlayerOverlay();
  }

  void _sendNextTour() {
    final webSocketService = Provider.of<WebSocketService>(context, listen: false);
    currentTour++;
    webSocketService.sendMessage(json.encode({
      'type': 'game_event',
      'event': 'TOUR_SUIVANT',
      'tour': currentTour,
    }));

    _showTourOverlay();
  }

  void _sendStartGame() {
    final webSocketService = Provider.of<WebSocketService>(context, listen: false);
    webSocketService.sendMessage(json.encode({
      'type': 'start',
      'message': 'START_GAME',
    }));

    setState(() {
      isGameStarted = true;
    });
  }

  @override
  void dispose() {
    _controller.dispose();
    _overlayController.dispose();
    _playerOverlayController.dispose();
    _tourOverlayController.dispose();
    _messageSubscription?.cancel();
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    final ws = Provider.of<WebSocketService>(context);

    return Scaffold(
      appBar: AppBar(
        title: const Text('Scores'),
        actions: [
          IconButton(
            icon: const Icon(Icons.navigate_next),
            onPressed: _sendNextPlayer,
          ),
          IconButton(
            icon: const Icon(Icons.autorenew),
            onPressed: _sendNextTour,
          )
        ],
      ),
      body: Stack(
        children: [
          _buildScoresBody(ws),
          if (showFinGameOverlay) _buildOverlay('Fin de partie', Colors.orange),
          if (showPlayerOverlay) _buildOverlay('Joueur Suivant: J$currentPlayer', Colors.blue),
          if (showTourOverlay) _buildOverlay('Tour $currentTour', Colors.green),
        ],
      ),
    );
  }

  Widget _buildScoresBody(WebSocketService ws) {
    // Corps principal générant les cartes de score
    return LayoutBuilder(
      builder: (context, constraints) {
        final spacing = constraints.maxWidth * 0.02;
        final usableHeight = constraints.maxHeight - spacing * 3;
        final cardHeight = (usableHeight / 2) - spacing;
        final cardWidth = (constraints.maxWidth - spacing * 3) / 2;
        final fontSize = cardHeight * 0.4;

        return FadeTransition(
          opacity: _fadeAnimation,
          child: ScaleTransition(
            scale: _scaleAnimation,
            child: ValueListenableBuilder<Map<int, String>>(
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
                            _buildScoreCard('${pseudos[0] ?? "J1"} : ${scores[0] ?? 0}', cardWidth, cardHeight, fontSize, const LinearGradient(colors: [Color(0xFFAEDCFA), Color(0xFF91C9F9)])),
                            SizedBox(width: spacing),
                            _buildScoreCard('${pseudos[1] ?? "J2"} : ${scores[1] ?? 0}', cardWidth, cardHeight, fontSize, const LinearGradient(colors: [Color(0xFF91C9F9), Color(0xFF6AB8F7)])),
                          ],
                        ),
                        SizedBox(height: spacing),
                        Row(
                          mainAxisAlignment: MainAxisAlignment.center,
                          children: [
                            _buildScoreCard('${pseudos[2] ?? "J3"} : ${scores[2] ?? 0}', cardWidth, cardHeight, fontSize, const LinearGradient(colors: [Color(0xFF6AB8F7), Color(0xFF429CF2)])),
                            SizedBox(width: spacing),
                            _buildScoreCard('${pseudos[3] ?? "J4"} : ${scores[3] ?? 0}', cardWidth, cardHeight, fontSize, const LinearGradient(colors: [Color(0xFF429CF2), Color(0xFF1E88E5)])),
                          ],
                        ),
                      ],
                    );
                  },
                );
              },
            ),
          ),
        );
      },
    );
  }

  Widget _buildOverlay(String text, Color color) {
    return FadeTransition(
      opacity: _playerOverlayController,
      child: ScaleTransition(
        scale: Tween<double>(begin: 0.9, end: 1.0).animate(
          CurvedAnimation(parent: _playerOverlayController, curve: Curves.easeOut),
        ),
        child: Container(
          width: MediaQuery.of(context).size.width * 0.75,
          height: MediaQuery.of(context).size.height * 0.75,
          decoration: BoxDecoration(
            color: color,
            borderRadius: BorderRadius.circular(20),
          ),
          alignment: Alignment.center,
          child: FittedBox(
            fit: BoxFit.scaleDown,
            child: Text(
              text,
              style: TextStyle(
                fontSize: MediaQuery.of(context).size.height * 0.1,
                fontWeight: FontWeight.bold,
                color: Colors.white,
              ),
            ),
          ),
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
}
