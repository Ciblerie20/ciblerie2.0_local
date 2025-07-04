String formatTimeMinSec(double seconds) {
  final minutes = seconds ~/ 60;
  final secs = seconds % 60;
  if (minutes > 0) {
    return '$minutes min ${secs.toStringAsFixed(3)} s';
  } else {
    return '${secs.toStringAsFixed(3)} s';
  }
}
