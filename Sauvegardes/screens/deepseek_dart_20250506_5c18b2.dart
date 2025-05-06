import 'package:flutter/material.dart';
import '../services/ip_manager.dart';

class ConfigScreen extends StatelessWidget {
  final _ipController = TextEditingController();

  ConfigScreen({Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Configuration IP')),
      body: FutureBuilder<String>( // Spécifiez explicitement le type String ici
        future: IPManager.getIP(),
        builder: (context, snapshot) {
          if (snapshot.hasData) {
            _ipController.text = snapshot.data!; // Ajout du ! pour confirmer que data n'est pas null
            return Padding(
              padding: const EdgeInsets.all(16.0),
              child: Column(
                children: [
                  TextField(
                    controller: _ipController,
                    decoration: const InputDecoration(
                      labelText: 'Adresse IP de l\'ESP32',
                      hintText: 'ws://192.168.1.100:81',
                    ),
                  ),
                  ElevatedButton(
                    onPressed: () async {
                      await IPManager.saveIP(_ipController.text);
                      Navigator.pop(context);
                    },
                    child: const Text('Enregistrer'),
                  ),
                ],
              ),
            );
          }
          return const Center(child: CircularProgressIndicator());
        },
      ),
    );
  }
}