# IR-2022-Projet


## Descriptions du projets

### Utilité de l'application

Le but du projet est d'utiliser [la base de données des arbres de Paris](https://opendata.paris.fr/explore/dataset/les-arbres/information/?disjunctive.typeemplacement&disjunctive.arrondissement&disjunctive.libellefrancais&disjunctive.genre&disjunctive.espece&disjunctive.varieteoucultivar&disjunctive.stadedeveloppement&disjunctive.remarquable) stockée au format CSV, afin d'en
extraire un arbre de poids minimal le poids d'un entre arbres étant ça distance ici (plan euclidien et sur une sphere).

### Structure de projet

La structure du projet est décomposé de cette façon :
```
├── include
├── src
├── tests
├── Makefile
├── Doxyfile
└── README.md
```
Où chaque ``.c`` est présent dans ``src`` et chaque ``.h`` dans ``include/``. Les tests unitaires sont présent dans ``tests/``.
 
## Exécution

### Utilisation de l'exécutable
Vous devez d'abord être à la racine pour compiler le projet avec la commande ``make``. Deux dossier seront créé : ``obj/`` et ``bin/``. L'exécutable ce trouve dans ``bin/``.

Pour lancer l'application: ``./bin/main [OPTIONS]``
L'ensembles des options est disponible avec la commande ``./bin/main --help``

Qui renvoie ceci :

```
Usage: main [OPTION...]
Give the Minimum Spanning Tree based on input file (normaly Tree's of Paris)
using Delaunay Triangulation and Prim algorithm.

  -d, --delimiter=DELIMITER  Delimiter for parsing input file. Default is ';'.
  -g, --debug                Produce debugging output
  -h, --height=HEIGHT        Height of the visualisation window. (default 900)
  -i, --input=CSV_FILE
  -l, --load delaunay=BINARY_FILE
                             Load delaunay triangles from binary file.
  -m, --save mst=PATH        Save mst to plain txt file with "org - dest"
                             convention.
  -o, --output=BINARY_FILE
  -s, --save delaunay=PATH   Save delaunay triangles to binary file.
  -v, --visualise            Visualise the result.
  -w, --width=WIDTH          Width of the visualisation window. (default 1400)
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version
```
Exemple pour utiliser l'exécutable :

- ``./bin/main -i ../les-arbres.csv -o coor.bin -g -v -l tests/files/delaunay/to_load/delaunay_256K
`` Lis le fichier présent dans ``../les-arbres`` sauvegarde le fichier binaire contenant les informations nécessaire dans ``coor.bin``, active les messages pour débugger et affiche le résultat dans une fênetre et lis le fichier binaire ``delaunay_256K`` où ce trouve la triangulation pré enregistré.

### Lancement des tests

Pour lancer les testes unitaires, vous pouvais les exécutées à partir de :
 - la racine du répertoire avec ``make test`` 
 - dans le dossier ``tests/`` avec ``make tests`` 

Vous pouvez aussi obtenir un code coverage avec ``make cov`` à la racine ou dans ``tests/``, un rapport sera alors généré dans ``tests/rapport`` ou ce trouvera un ``index.html`` contenant le code coverage.

*Note: il est nécessaire d'avoir lcov d'installé sur votre machine.* 

### Génération de la documentation
Il suffit d'exécuter ``make doc`` à la racine du projet, un dossier ``docs`` sera alors généré et la documentation serra accessible via ``docs/html/index.html``.

## Note
La librairie argp est utilisé dans ce projet, il ce peut que sur certaine platforme le projet ne compile pas dû au faite que cette librairie est peut être manquante.

## Auteurs
* Loïc Herlin
* Alexander Yanovskyy
