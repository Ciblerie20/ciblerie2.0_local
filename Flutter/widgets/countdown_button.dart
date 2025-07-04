import 'package:flutter/material.dart';

class CountdownButton extends StatefulWidget {
  final ValueNotifier<double> countdownNotifier;
  const CountdownButton({required this.countdownNotifier, super.key});

  @override
  State<CountdownButton> createState() => _CountdownButtonState();
}

class _CountdownButtonState extends State<CountdownButton> {
  double _lastNonNegative = 0;

  @override
  void initState() {
    super.initState();
    widget.countdownNotifier.addListener(_onCountdown);
    _lastNonNegative = widget.countdownNotifier.value;
  }

  @override
  void dispose() {
    widget.countdownNotifier.removeListener(_onCountdown);
    super.dispose();
  }

  void _onCountdown() {
    if (widget.countdownNotifier.value >= 0) {
      setState(() {
        _lastNonNegative = widget.countdownNotifier.value;
      });
    }
  }

  @override
  Widget build(BuildContext context) {
    final double countdown = widget.countdownNotifier.value;
    // Affiche le bouton si countdown >= 0, ou si la dernière valeur connue était 0.000
    if (countdown >= 0 || _lastNonNegative == 0.0) {
      final display = countdown >= 0 ? countdown : 0.0;
      print('DEBUG Flutter AFFICHAGE: $countdown');
      return Padding(
        padding: const EdgeInsets.symmetric(horizontal: 8.0),
        child: AnimatedSwitcher(
          duration: const Duration(milliseconds: 150),
          child: ElevatedButton(
            key: const ValueKey('countdown'),
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
              '⏳ ${display.toStringAsFixed(3)} s',
              style: const TextStyle(
                fontWeight: FontWeight.bold,
                color: Colors.black,
              ),
            ),
          ),
        ),
      );
    } else {
      return const SizedBox(
        key: ValueKey('empty'),
        width: 110,
        height: 40,
      );
    }
  }
}
