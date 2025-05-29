import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../services/websocket_service.dart';

class PseudoConfigScreen extends StatefulWidget {
  const PseudoConfigScreen({Key? key}) : super(key: key);

  @override
  _PseudoConfigScreenState createState() => _PseudoConfigScreenState();
}

class _PseudoConfigScreenState extends State<PseudoConfigScreen> {
  final List<TextEditingController> _controllers = List.generate(4, (_) => TextEditingController());

  @override
  void initState() {
    super.initState();
    final ws = Provider.of<WebSocketService>(context, listen: false);
    for (int i = 0; i < _controllers.length; i++) {
      _controllers[i].text = ws.pseudonymsNotifier.value[i] ?? 'J${i + 1}';
    }
  }

  @override
  Widget build(BuildContext context) {
    final ws = Provider.of<WebSocketService>(context);

    return Scaffold(
      appBar: AppBar(
        title: const Text('Configuration des Pseudos'),
        centerTitle: true,
      ),
      body: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.spaceBetween,
          children: [
            Expanded(
              child: ListView.builder(
                itemCount: 4,
                itemBuilder: (context, i) {
                  return Padding(
                    padding: const EdgeInsets.only(bottom: 15.0),
                    child: TextField(
                      controller: _controllers[i],
                      decoration: InputDecoration(
                        labelText: 'Joueur ${i + 1}',
                        border: const OutlineInputBorder(),
                      ),
                    ),
                  );
                },
              ),
            ),
            ElevatedButton(
              style: ElevatedButton.styleFrom(
                minimumSize: const Size(double.infinity, 50),
                backgroundColor: const Color(0xFF7DBFF8),
              ),
              onPressed: () {
                final pseudos = _getValidatedPseudos();
                ws.setPseudonyms(pseudos);

                // Redirection vers ScoresScreen
                Navigator.pushReplacementNamed(context, '/scores');
              },
              child: const Text(
                'Valider les pseudos',
                style: TextStyle(fontWeight: FontWeight.bold, color: Colors.black),
              ),
            ),
          ],
        ),
      ),
    );
  }

  /// Valide les champs de texte et retourne un Map avec les pseudos
  Map<int, String> _getValidatedPseudos() {
    return {
      for (int i = 0; i < _controllers.length; i++)
        i: _controllers[i].text.isNotEmpty ? _controllers[i].text : 'J${i + 1}',
    };
  }

  @override
  void dispose() {
    for (var controller in _controllers) {
      controller.dispose();
    }
    super.dispose();
  }
}
