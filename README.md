# Magic

<p float="left">
  <a href="https://ubuntu.com/">
    <img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black"/>
  </a>
  <a href="https://code.visualstudio.com/">
    <img src="https://img.shields.io/badge/VSCode-0078D4?style=for-the-badge&logo=visual%20studio%20code&logoColor=white"/>
  </a>
  <a href="https://cplusplus.com/doc/tutorial/">
    <img src="https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white"/>
  </a>
  <a href="https://junit.org/junit5/">
      <img src="https://img.shields.io/badge/Junit5-25A162?style=for-the-badge&logo=junit5&logoColor=white"/>
  </a>
</p>

Ce projet est une version extrêmement simpliée du jeu de carte à collectionner Magic The Gathering. <br>

## Fonctionnalités implémentées

Au lancement du jeu, plusieurs modes sont disponibles :  

<img src="https://user-images.githubusercontent.com/82103105/181829610-05c7daf4-7228-4265-a89f-3fdedff93c48.png" width="400">

Il est alors possible de lancer une nouvelle partie et de la sauvegarder à tout moment, de charger une partie sauvegardée, de créer son deck et ses cartes (il peut être créé en plusieurs fois : le sauvegarder et le charger), de quitter le jeu.

#### Déroulement d'une partie

1. Lancement d’une partie avec deux joueurs avec 20 points de vie initiaux;
2. Choix des decks, mélange des decks et distribution des cartes;
3. Déroulement du jeu; <br>
  a- Phase de pioche; <br>
  b- Phase de désengagement; <br>
  c- Phase principale; <br>
  d- Phase de combat; <br>
  e- Phase secondaire; <br>
  f- Fin de tour et changement du joueur actif. <br>
4. Fin de partie.

#### Affichage durant la partie



## Mode d'emploi

Dans le dossier du projet, on retrouve un Makefile permettant de faire les commandes suivantes : <br>
```
• make : compile le programme
• make run : lance le jeu
• make clean : efface tous les dossiers et fichiers générés pendant la compilation 
```
