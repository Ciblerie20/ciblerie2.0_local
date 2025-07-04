import 'dart:math';
import 'package:flutter/material.dart';

class InteractiveTargets extends StatefulWidget {
  const InteractiveTargets({super.key});

  @override
  State<InteractiveTargets> createState() => _InteractiveTargetsState();
}

class _InteractiveTargetsState extends State<InteractiveTargets> {
  // Pour l’exemple, garder l’état de sélection (tu peux l’enlever si inutile)
  int? selectedIndex;

  @override
  Widget build(BuildContext context) {
    return LayoutBuilder(
      builder: (context, constraints) {
        // Utiliser le plus petit côté pour garder le schéma centré et proportionné
        final double size = min(constraints.maxWidth, constraints.maxHeight);
        final Offset center = Offset(constraints.maxWidth/2, constraints.maxHeight/2);

        // Rayon des anneaux
        final double rSmall = size * 0.18;   // 4 cibles
        final double rBig   = size * 0.45;   // 8 cibles

        // Rayon des cibles
        final double cibleRadius = size * 0.058;

        // Cibles : index 0 = centre, 1-4 = petit cercle, 5-12 = grand cercle
        List<Widget> targets = [];

        // Centre
        targets.add(_CibleNoire(
          center: center,
          radius: cibleRadius,
          onTap: () {
            setState(() => selectedIndex = 0);
            debugPrint('Cible centrale tapée !');
          },
          selected: selectedIndex == 0,
        ));

        // 4 cibles sur petit cercle (haut, droite, bas, gauche)
        for (int i = 0; i < 4; i++) {
          final double angle = pi/2 * i; // 0, 90, 180, 270°
          final Offset pos = center + Offset(rSmall * cos(angle - pi/2), rSmall * sin(angle - pi/2));
          targets.add(_CibleColor(
            center: pos,
            radius: cibleRadius,
            onTap: () {
              setState(() => selectedIndex = i + 1);
              debugPrint('Cible ${i+1} tapée !');
            },
            selected: selectedIndex == i + 1,
          ));
        }

        // 8 cibles sur grand cercle (tous les 45°)
        for (int i = 0; i < 8; i++) {
          final double angle = pi/4 * i; // 0, 45, ..., 315°
          final Offset pos = center + Offset(rBig * cos(angle - pi/2), rBig * sin(angle - pi/2));
          targets.add(_CibleColor(
            center: pos,
            radius: cibleRadius,
            onTap: () {
              setState(() => selectedIndex = i + 5);
              debugPrint('Cible ${i+5} tapée !');
            },
            selected: selectedIndex == i + 5,
          ));
        }

        // Les anneaux de repère (optionnel)
        final List<double> rings = [rBig, rSmall];
        List<Widget> ringsWidgets = rings.map((r) => Positioned(
          left: center.dx - r,
          top: center.dy - r,
          width: 2*r,
          height: 2*r,
          child: IgnorePointer(
            child: Container(
              decoration: BoxDecoration(
                shape: BoxShape.circle,
                color: Colors.grey.withOpacity(0.18),
              ),
            ),
          ),
        )).toList();

        return Stack(
          children: [
            ...ringsWidgets,
            ...targets,
          ],
        );
      },
    );
  }
}

// Cible centrale noire
class _CibleNoire extends StatelessWidget {
  final Offset center;
  final double radius;
  final VoidCallback onTap;
  final bool selected;
  const _CibleNoire({
    required this.center,
    required this.radius,
    required this.onTap,
    required this.selected,
  });

  @override
  Widget build(BuildContext context) {
    return Positioned(
      left: center.dx - radius,
      top: center.dy - radius,
      width: 2*radius,
      height: 2*radius,
      child: GestureDetector(
        onTap: onTap,
        child: Stack(
          alignment: Alignment.center,
          children: [
            AnimatedContainer(
              duration: const Duration(milliseconds: 150),
              decoration: BoxDecoration(
                shape: BoxShape.circle,
                color: Colors.black,
                border: selected ? Border.all(color: Colors.amber, width: 4) : null,
              ),
            ),
            // 3 petits points bleus
            Positioned(
              top: radius * 0.45,
              left: radius * 0.35,
              child: _PetitPointBleu(radius: radius * 0.19),
            ),
            Positioned(
              top: radius * 0.45,
              right: radius * 0.35,
              child: _PetitPointBleu(radius: radius * 0.19),
            ),
            Positioned(
              bottom: radius * 0.22,
              child: _PetitPointBleu(radius: radius * 0.19),
            ),
          ],
        ),
      ),
    );
  }
}

class _PetitPointBleu extends StatelessWidget {
  final double radius;
  const _PetitPointBleu({required this.radius});
  @override
  Widget build(BuildContext context) {
    return Container(
      width: 2*radius,
      height: 2*radius,
      decoration: BoxDecoration(
        color: Color(0xFF0033CC),
        shape: BoxShape.circle,
      ),
    );
  }
}

// Cible colorée (rouge/bleu)
class _CibleColor extends StatelessWidget {
  final Offset center;
  final double radius;
  final VoidCallback onTap;
  final bool selected;
  const _CibleColor({
    required this.center,
    required this.radius,
    required this.onTap,
    required this.selected,
  });

  @override
  Widget build(BuildContext context) {
    return Positioned(
      left: center.dx - radius,
      top: center.dy - radius,
      width: 2*radius,
      height: 2*radius,
      child: GestureDetector(
        onTap: onTap,
        child: AnimatedContainer(
          duration: const Duration(milliseconds: 150),
          decoration: BoxDecoration(
            shape: BoxShape.circle,
            border: selected ? Border.all(color: Colors.amber, width: 4) : null,
          ),
          child: CustomPaint(
            painter: _CiblePainter(),
            size: Size.square(2*radius),
          ),
        ),
      ),
    );
  }
}

class _CiblePainter extends CustomPainter {
  @override
  void paint(Canvas canvas, Size size) {
    final double r = size.width/2;
    final Offset c = Offset(r, r);
    // 3 cercles concentriques (bleu, rouge)
    canvas.drawCircle(c, r, Paint()..color = Color(0xFF0033CC));
    canvas.drawCircle(c, r*0.65, Paint()
      ..color = Colors.transparent
      ..strokeWidth = r*0.35
      ..style = PaintingStyle.stroke
      ..shader = null
      ..blendMode = BlendMode.srcOver
      ..strokeCap = StrokeCap.round
      ..strokeJoin = StrokeJoin.round
      ..color = Colors.red
    );
    canvas.drawCircle(c, r*0.40, Paint()..color = Color(0xFF0033CC));
    canvas.drawCircle(c, r*0.22, Paint()
      ..color = Colors.red
      ..style = PaintingStyle.stroke
      ..strokeWidth = r*0.18
    );
    canvas.drawCircle(c, r*0.10, Paint()..color = Color(0xFF0033CC));
  }

  @override
  bool shouldRepaint(covariant CustomPainter oldDelegate) => false;
}
