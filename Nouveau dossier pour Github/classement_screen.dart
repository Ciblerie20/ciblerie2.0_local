import 'package:flutter/material.dart';

class ClassementScreen extends StatelessWidget {
  const ClassementScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Classement'),
      ),
      body: const Center(
        child: Text('Page Classement'),
      ),
    );
  }
}