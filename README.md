# Scrabble

## English

This program is a C++ implementation of the Scrabble board game, playable in terminal.
This version of the program enforces additional game rules; tiles must be placed according to the game rules and created words are checked against a dictionary. 

Gameplay error messages are now context sensitive and verbose. 
Support for three and four player modes of play have been added.

-   save\_<filename> - save files
-   \*.cpp - code files
-   \*.h - header files
-   ScrabbleTiles.txt - the different tiles used in the game.
-   Makefile - C++ compilation file (make)

Compile with `make` and run with `./scrabble`. 
Additional command line arguments:
  
-   -3player\
  enables three player mode
-   -4player\
  enables four player mode
-   -betterInvalidInput\
  enables verbose errors
-   -dictionary\
  enables dictionary word checking

Test information in tests/TESTING_README.md
  
## Français

Cette application est une implémentation du jeu de plateau Scrabble, jouable dans le terminal à code.
  
Cette version de l'application applique des règles additionnelles; les teuiles doivent se mettre aux règles du jeu, et les mots fabriqués par les joueurs sont verifiés par rapport à un dictionnaire.

Messages d'erreurs en jouant sont sensibles au contexte.
Façons de jouer avec trois et quatre joueurs sont ajoutées.

-   save\_<filename> - sauvegardes
-   \*.cpp - fichiers de code
-   \*.h - fichiers headers
-   ScrabbleTiles.txt - les teuiles differentes s'utilisent dans le jeu
-   Makefile - compilateur (make)

Paramètres additionnels d'execution:
  
-   -3player\
  pour soutiener trois joueurs
-   -4player\
  pour soutiener quatre joueurs
-   -betterInvalidInput\
  activer les messages d'erreurs verbeuses
-   -dictionary\
  activer la vérification des mots

L'information sur les tests sont dans tests/TESTING_README.md
