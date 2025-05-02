# Documentation Complète : Gestion des Scores avec Arduino Mega, ESP32, et Supabase

## **Table des Matières**
1. [Objectifs du Projet](#objectifs-du-projet)
2. [Architecture Globale](#architecture-globale)
3. [Schéma Visuel](#schéma-visuel)
4. [Détails des Composants](#détails-des-composants)
    - [Arduino Mega](#1-arduino-mega)
    - [ESP32 (Cloud)](#2-esp32-cloud)
    - [ESP32 (WebSocket)](#3-esp32-websocket)
    - [Supabase (Base de Données)](#4-supabase-base-de-données)
    - [Site Web Local](#5-site-web-local)
    - [Site Principal (Render)](#6-site-principal-render)
5. [Optimisation des Consommations](#optimisation-des-consommations)
6. [Prochaines Étapes](#prochaines-étapes)

---

## **Objectifs du Projet**
Ce projet vise à créer un système complet de gestion de scores pour un jeu, intégrant les éléments suivants :
- **Fonctionnement 100% local** pour 1 à 4 joueurs.
- Affichage des scores en temps réel via un **site web local**.
- Transmission sécurisée des données vers une base de données distante (**Supabase**).
- Classements globaux consultables sur un **site principal hébergé sur Render**.
- Protection contre la triche et optimisation des consommations réseau.

---

## **Architecture Globale**

| **Composant**             | **Rôle**                                                                                       | **Technologie utilisée**            |
|----------------------------|-----------------------------------------------------------------------------------------------|-------------------------------------|
| **Arduino Mega**           | Gère la logique du jeu, collecte les scores, et les transmet à un ESP32.                      | Arduino Mega                        |
| **ESP32 (Cloud)**          | Envoie les données (scores et pseudos) vers une base de données distante (Supabase).          | ESP32                               |
| **ESP32 (WebSocket)**      | Héberge un serveur WebSocket local pour transmettre les scores en temps réel au site local.   | ESP32                               |
| **Base de données (Cloud)**| Stocke les scores et statistiques des joueurs.                                                | Supabase (ou Firestore)             |
| **Site Web Local**         | Affiche les scores et statistiques en direct.                                                 | Node.js et Flutter (Frontend)       |
| **Site Principal (Render)**| Affiche les classements globaux accessibles à tous.                                           | Render (hébergement) et Supabase    |

---

## **Schéma Visuel**

### **Diagramme général de l'architecture**

```mermaid
graph TD
    A[Arduino Mega] -->|UART| B[ESP32 (Cloud)]
    B -->|HTTPS| C[Supabase (Base de Données)]
    B -->|WiFi| D[ESP32 (WebSocket)]
    D -->|WebSocket| E[Site Web Local]
    C -->|Requête API| F[Site Principal (Render)]
    E -->|Affichage| F
```

---

## **Détails des Composants**

### **1️⃣ Arduino Mega**
- **Rôle :** Microcontrôleur principal qui gère la logique du jeu (pseudos, scores, statistiques).
- **Données transmises :**
  - Format JSON pour une communication structurée :
    ```json
    {
      "pseudo": "Joueur1",
      "score": 120,
      "stats": {
        "cibles_touchees": 10,
        "temps_total": 300
      }
    }
    ```
- **Transmission :** Envoie les données au premier ESP32 via UART (ou SPI/I2C).

---

### **2️⃣ ESP32 (Cloud)**

| **Caractéristique**       | **Détails**                                                                                   |
|----------------------------|-----------------------------------------------------------------------------------------------|
| **Rôle**                  | Passerelle entre l'Arduino Mega et Supabase.                                                  |
| **Fonctionnalités**       | Envoie les données des joueurs vers Supabase via HTTPS.                                       |
| **Sécurisation**          | Utilise une clé API pour authentifier les requêtes.                                           |
| **Exemple de Requête HTTP**|                                                                                              |
| **Optimisation**          | Grouper les données pour réduire les appels réseau.                                           |

**Exemple de requête HTTP vers Supabase :**
```json
POST
```

```plaintext
POST /rest/v1/scores HTTP/1.1
````markdown name=project_architecture_with_diagrams.md
Host: your-supabase-instance.supabase.co
Authorization: Bearer <API_KEY>
Content-Type: application/json

{
  "pseudo": "Joueur1",
  "score": 120,
  "cibles_touchees": 10,
  "temps_total": 300
}
```
---

### **3️⃣ ESP32 (WebSocket)**

| **Caractéristique**       | **Détails**                                                                                   |
|----------------------------|-----------------------------------------------------------------------------------------------|
| **Rôle**                  | Héberge un serveur WebSocket local pour transmettre les scores en temps réel.                |
| **Fonctionnalités**       | Permet au site web local d'afficher les scores en direct.                                    |
| **Exemple de Message WebSocket** |                                                                                        |
| **Avantages**             | Fonctionne sans besoin de connexion Internet.                                                |

**Exemple de message WebSocket envoyé par l'ESP32 :**
```json
{
  "type": "update",
  "data": {
    "pseudo": "Joueur1",
    "score": 120,
    "statistiques": {
      "cibles_touchees": 10,
      "temps_total": 300
    }
  }
}
```

---

### **4️⃣ Supabase (Base de Données)**

| **Caractéristique**       | **Détails**                                                                                   |
|----------------------------|-----------------------------------------------------------------------------------------------|
| **Pourquoi Supabase ?**   | Gestion SQL pour relations complexes et support du temps réel.                               |
| **Schéma de la Base de Données** |                                                                                      |
| **Optimisation**          | Créer des index pour accélérer les requêtes complexes.                                       |

#### **Schéma de la base de données :**
**Table `joueurs`**
| **Colonne**     | **Type**         | **Description**               |
|------------------|------------------|-------------------------------|
| `id`            | `serial`         | Identifiant unique.           |
| `pseudo`        | `varchar(50)`    | Nom du joueur.                |
| `email`         | `varchar(100)`   | Adresse email (optionnel).    |

**Table `scores`**
| **Colonne**     | **Type**         | **Description**               |
|------------------|------------------|-------------------------------|
| `id`            | `serial`         | Identifiant unique du score.  |
| `joueur_id`     | `int`            | Référence à `joueurs.id`.     |
| `score`         | `int`            | Score obtenu.                 |
| `cibles_touchees`| `int`            | Nombre de cibles touchées.    |
| `temps_total`   | `int`            | Temps total de la partie.     |

---

### **5️⃣ Site Web Local**

#### **Caractéristiques du Site Web Local**
| **Technologie**                     | **Détails**                                                                                  |
|-------------------------------------|---------------------------------------------------------------------------------------------|
| Backend                             | Node.js ou Python pour gérer la logique locale.                                             |
| Frontend                            | Flutter pour une interface utilisateur interactive et moderne.                              |
| Connexion avec ESP32 (WebSocket)    | Reçoit les données en temps réel via WebSocket.                                             |
| Fonctionnalité                      | Affiche les scores, statistiques et tableau en direct pour 1 à 4 joueurs.                  |

#### **Exemple d'affichage Flutter :**
Un tableau de scores avec les colonnes suivantes :
| **#** | **Pseudo**  | **Score**  | **Cibles Touchées** | **Temps Total** |
|-------|-------------|------------|---------------------|-----------------|
| 1     | Joueur1     | 120        | 10                  | 300 secondes    |
| 2     | Joueur2     | 95         | 8                   | 250 secondes    |

---

### **6️⃣ Site Principal (Render)**

| **Caractéristique**       | **Détails**                                                                                   |
|----------------------------|-----------------------------------------------------------------------------------------------|
| **Hébergement**           | Render, avec une connexion directe à Supabase.                                                |
| **Fonctionnalités**       | Classements globaux, meilleurs joueurs, statistiques générales des parties.                    |
| **Optimisation**          | Charger les données en lots pour minimiser les appels API.                                    |

---

## **Optimisation des Consommations**

| **Composant**             | **Optimisation proposée**                                                                    |
|----------------------------|---------------------------------------------------------------------------------------------|
| **ESP32 (Cloud)**          | Grouper les données en lots avant de les envoyer.                                           |
| **ESP32 (WebSocket)**      | Envoyer uniquement les mises à jour critiques.                                              |
| **Supabase**               | Créer des index SQL pour accélérer les requêtes.                                            |
| **Site Web Local**         | Pré-calculer les données localement pour éviter les dépendances réseau inutiles.            |
| **Site Principal (Render)**| Réduire la fréquence de mise à jour en fonction des besoins de l'utilisateur.               |

---

## **Prochaines Étapes**

1. **Configurer l'ESP32 (Cloud) :**
   - Implémenter l'envoi des données vers Supabase via HTTPS.
   - Ajouter une clé API pour sécuriser les requêtes.

2. **Développer l'ESP32 (WebSocket) :**
   - Mettre en place un serveur WebSocket local.
   - Tester les communications avec le site web local.

3. **Créer le Site Web Local :**
   - Développer une interface simple avec Flutter.
   - Connecter le site au WebSocket pour les mises à jour en direct.

4. **Finaliser le Site Principal (Render) :**
   - Connecter Render à Supabase pour récupérer les données.
   - Créer une page interactive affichant les classements globaux.

---

## **Conclusion**

Ce document fournit une vue d'ensemble détaillée, avec des schémas, des tableaux, et des étapes claires à suivre. Si vous avez besoin de code pour un composant spécifique (comme l'ESP32 ou l'intégration Supabase), n'hésitez pas à demander ! 😊