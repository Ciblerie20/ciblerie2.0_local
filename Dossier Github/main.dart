import 'package:flutter/material.dart';
import 'screens/home_screen.dart';

void main() {
  runApp(const CiblerieApp());
}

class CiblerieApp extends StatelessWidget {
  const CiblerieApp({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Ciblerie 2.0',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: const HomeScreen(),
    );
  }
}