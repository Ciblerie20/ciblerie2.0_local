// 🎯 Version fusionnée complète avec Supabase + Providers + Login + Pseudo + Session persistante + Accueil personnalisé

import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'package:supabase_flutter/supabase_flutter.dart';

import 'services/websocket_service.dart';
import 'services/theme_notifier.dart';
import 'models/group_model.dart';
import 'models/game_ui_state.dart';
import 'screens/home_screen.dart';
import 'screens/scores_screen.dart';
import 'package:provider/provider.dart' as provider_pkg;  // alias ici

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Supabase.initialize(
    url: 'https://bnlhtabceluzevevxwqu.supabase.co',
    anonKey: '<eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImJubGh0YWJjZWx1emV2ZXZ4d3F1Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3NTA5NTcyMjEsImV4cCI6MjA2NjUzMzIyMX0.aPFIZGFVHkx1ccbxZrFXnHt1a45pSgWy33pOf3gysaY>',
  );

  runApp(
    MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => WebSocketService()),
        ChangeNotifierProvider(create: (_) => ThemeNotifier()),
        ChangeNotifierProvider(create: (_) => GroupModel()),
        ChangeNotifierProvider(create: (_) => GameUIState()),
      ],
      child: const MyApp(),
    ),
  );
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    final themeNotifier = provider_pkg.Provider.of<ThemeNotifier>(context);

    return MaterialApp(
      title: 'Ciblerie',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.blue,
        brightness: Brightness.light,
        appBarTheme: const AppBarTheme(
          backgroundColor: Colors.white,
          foregroundColor: Colors.black87,
        ),
      ),
      darkTheme: ThemeData(
        brightness: Brightness.dark,
        appBarTheme: const AppBarTheme(
          backgroundColor: Color(0xFF1E1E1E),
          foregroundColor: Colors.white,
        ),
      ),
      themeMode: themeNotifier.themeMode,
      home: const AuthGate(),
      routes: {
        '/scores': (context) => const ScoresScreen(),
      },
    );
  }
}

class AuthGate extends StatefulWidget {
  const AuthGate({super.key});

  @override
  State<AuthGate> createState() => _AuthGateState();
}

class _AuthGateState extends State<AuthGate> {
  @override
  Widget build(BuildContext context) {
    final session = Supabase.instance.client.auth.currentSession;
    return session != null ? const HomeScreen() : const LoginPage();
  }
}

class LoginPage extends StatefulWidget {
  const LoginPage({super.key});

  @override
  State<LoginPage> createState() => _LoginPageState();
}

class _LoginPageState extends State<LoginPage> {
  final emailController = TextEditingController();
  final pseudoController = TextEditingController();
  final supabase = Supabase.instance.client;

  Future<void> signIn() async {
    final email = emailController.text.trim();
    final pseudo = pseudoController.text.trim();
    if (email.isEmpty || pseudo.isEmpty) return;

    try {
      await supabase.auth.signInWithOtp(email: email);
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('🔗 Lien envoyé par email.')), // email OTP/magic link
      );
    } catch (e) {
      if (!mounted) return;
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(content: Text('❌ Erreur : $e')),
      );
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('Connexion Joueur')),
      body: Padding(
        padding: const EdgeInsets.all(16),
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            TextField(
              controller: emailController,
              decoration: const InputDecoration(
                labelText: 'Adresse email',
                border: OutlineInputBorder(),
              ),
              keyboardType: TextInputType.emailAddress,
            ),
            const SizedBox(height: 12),
            TextField(
              controller: pseudoController,
              decoration: const InputDecoration(
                labelText: 'Pseudo',
                border: OutlineInputBorder(),
              ),
            ),
            const SizedBox(height: 20),
            ElevatedButton(
              onPressed: signIn,
              child: const Text('Se connecter'),
            ),
          ],
        ),
      ),
    );
  }
}
