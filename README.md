# Projet de Compilation d'Automates

## Description
Ce projet implémente un compilateur complet pour un langage de description d'automates finis.
Le compilateur est composé de plusieurs phases principales :
- **Analyse Lexicale** – tokenisation du texte d'entrée
- **Analyse Syntaxique** – validation de la grammaire et de la structure
- **Analyse Sémantique** – vérification de la cohérence logique de l'automate
- **Algorithmes sur Automates** – simulation, déterminisation, et manipulation
- **Génération de Code** – production de code C exécutable

L'objectif du projet est de garantir qu'un automate décrit par l'utilisateur soit syntaxiquement correct, sémantiquement valide, et formellement cohérent, tout en offrant des outils pour le manipuler.

---

## Fonctionnalités

### Analyseur Lexical (Lexer)
* Reconnaît les **mots-clés** : `etats`, `alphabet`, `initial`, `final`, `transitions`
* Reconnaît les **identificateurs** pour les états et les symboles
* Gère les **symboles spéciaux** : `=`, `,`, `;`, `{`, `}`, `->`, `:`
* Ignore les espaces blancs et les commentaires optionnels (`//`)
* Produit une **liste de lexèmes et leurs types de tokens**

### Analyseur Syntaxique (Parser)
* Valide les règles de syntaxe : ordre et format corrects des sections de l'automate
* Détecte les sections manquantes : états, alphabet, initial, final, transitions
* Parse les définitions d'états : `etats = q0,q1,q2;`
* Parse les définitions d'alphabet : `alphabet = a,b;`
* Parse l'état initial : `initial = q0;`
* Parse les états finaux : `final = q2;`
* Parse les transitions : `q0 -> q1 : a;`
* Fournit des messages d'erreur clairs en cas de violation de syntaxe

### Analyseur Sémantique

L'analyseur sémantique garantit la cohérence logique et formelle de l'automate après le parsing.

**Responsabilités :**
* Vérifier que l'état initial existe
* Vérifier que tous les états finaux sont définis
* S'assurer que les transitions utilisent des états existants
* S'assurer que les symboles de transition appartiennent à l'alphabet
* Détecter les automates non-déterministes
  
**Règles Sémantiques Vérifiées :**
* L'état initial doit appartenir à l'ensemble des états
* Les états finaux doivent être des états définis
* Tous les symboles de transition doivent être déclarés dans l'alphabet
* Chaque transition doit lier des états valides
* Un avertissement est généré en cas de non-déterminisme

### Algorithmes sur Automates

Le compilateur implémente plusieurs algorithmes classiques :

* **Simulation d'Automate** : teste si une chaîne est acceptée par l'automate
* **Test de Déterminisme** : détecte les conflits de transitions
* **Déterminisation** : transforme un automate non-déterministe en déterministe (construction des sous-ensembles avec macro-états)
* **Affichage Formaté** : présente la structure complète de l'automate
* **Sauvegarde** : exporte l'automate dans un fichier au format du langage

### Génération de Code C

Le générateur de code produit un fichier C contenant :
* Une structure statique initialisée avec les données de l'automate
* Une fonction `get_automate()` pour récupérer l'automate
* Code prêt à être compilé et intégré dans d'autres projets

---



## Compilation

### Compilation Complète (Recommandée)

```bash
make
```

Cette commande compile l'ensemble du projet et génère l'exécutable `aut`.



---

## Utilisation

### Afficher l'Aide

```bash
./aut help
```

### Commandes Disponibles

#### 1. Compiler un Automate
Vérifie la syntaxe et la sémantique de l'automate :
```bash
./aut compile <fichier>
```
**Exemple :**
```bash
./aut compile tests/automate.txt
```

#### 2. Afficher un Automate
Affiche la structure complète de l'automate :
```bash
./aut afficher <fichier>
```

#### 3. Tester une Chaîne
Teste si une chaîne est acceptée par l'automate :
```bash
./aut accepter <fichier> <chaine>
```
**Exemple :**
```bash
./aut accepter tests/automate.txt "aab"
```

#### 4. Déterminiser un Automate
Transforme l'automate en automate déterministe :
```bash
./aut determiniser <fichier> [--save <fichier_sortie>]
```
**Exemples :**
```bash
./aut determiniser tests/automate_nd.txt
./aut determiniser tests/automate_nd.txt --save deterministe.txt
```

#### 5. Générer du Code C
Génère un fichier C contenant la structure de l'automate :
```bash
./aut generer <fichier> [nom_sortie]
```
**Exemple :**
```bash
./aut generer tests/automate.txt automate_data.c
```

---

## Format du Langage

### Syntaxe

```
etats = q0, q1, q2;
alphabet = a, b;
initial = q0;
final = q2;
transitions {
    q0 -> q1 : a;
    q1 -> q2 : b;
    q0 -> q2 : a, b;
}
```

### Règles

* **États** : Identificateurs libres (lettres, chiffres, underscores)
* **Symboles** : Un seul caractère alphabétique
* **Commentaires** : Ligne commençant par `//`
* **Sections obligatoires** : Les 5 sections doivent être présentes
* **Ordre** : L'ordre des sections n'a pas d'importance
* **Transitions multiples** : Plusieurs symboles peuvent partager la même transition

---

## Exemples

### Exemple 1 : Automate Déterministe Simple

**Fichier : `exemple_deterministe.txt`**
```
etats = q0, q1, q2;
alphabet = a, b;
initial = q0;
final = q2;
transitions {
    q0 -> q1 : a;
    q1 -> q2 : b;
}
```

**Test :**
```bash
./aut compile exemple_deterministe.txt
./aut accepter exemple_deterministe.txt "ab"
```

### Exemple 2 : Automate Non-Déterministe

**Fichier : `exemple_nondeterministe.txt`**
```
etats = q0, q1, q2;
alphabet = a, b;
initial = q0;
final = q2;
transitions {
    q0 -> q1 : a;
    q0 -> q2 : a;  // Conflit : deux destinations pour (q0, a)
    q1 -> q2 : b;
}
```

**Déterminisation :**
```bash
./aut determiniser exemple_nondeterministe.txt --save deterministe.txt
```

---

## Résultats Attendus

### Automates Valides
* Acceptés et analysés avec succès
* Affichage de la structure complète
* Algorithmes applicables (simulation, déterminisation)

### Erreurs de Syntaxe
* Arrêt de la compilation avec messages clairs
* Indication du token problématique
* Position de l'erreur

### Erreurs Sémantiques
* Détection et rapport des incohérences logiques
* États non définis
* Symboles non déclarés
* Transitions invalides

### Automates Non-Déterministes
* Génération d'avertissements
* Possibilité de déterminisation automatique
* Conservation de l'équivalence sémantique

---

## Limitations Actuelles

* **Taille maximale** : 50 états, 200 transitions
* **Noms d'états** : Maximum 20 caractères (100 pour macro-états)
* **Alphabet** : Limité à 26 symboles
* **Pas de support** : ε-transitions (transitions vides)