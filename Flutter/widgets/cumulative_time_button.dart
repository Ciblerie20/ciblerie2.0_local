import 'package:flutter/material.dart';

class CumulativeTimeButton extends StatelessWidget {
  final ValueNotifier<double> cumulativeNotifier;
  const CumulativeTimeButton({required this.cumulativeNotifier, super.key});

  @override
  Widget build(BuildContext context) {
    return ValueListenableBuilder<double>(
      valueListenable: cumulativeNotifier,
      builder: (context, cumulative, _) {
        return Padding(
          padding: const EdgeInsets.symmetric(horizontal: 8.0),
          child: ElevatedButton(
            onPressed: null,
            style: ElevatedButton.styleFrom(
              backgroundColor: const Color(0xFFB5E7A0),
              disabledBackgroundColor: const Color(0xFFB5E7A0),
              elevation: 0,
              shape: RoundedRectangleBorder(
                borderRadius: BorderRadius.circular(20),
              ),
            ),
            child: Text(
              '⏱️ ${cumulative.toStringAsFixed(3)} s',
              style: const TextStyle(
                fontWeight: FontWeight.bold,
                color: Colors.black,
              ),
            ),
          ),
        );
      },
    );
  }
}
