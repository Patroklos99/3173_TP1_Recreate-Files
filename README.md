# TP0 - `wall-e`

L'objectif du TP0 est de développer l'utilitaire `wall-e` qui lit un fichier au format `stat` et recréé les fichiers, les répertoires et liens symboliques de ce fichier.


<summary>Exemple</summary>

<pre>
<b>iam@groot:~/$</b> ls tests/
output.stat
<b>iam@groot:~/$</b> cat tests/output.stat
lrwxrwxrwx 6 lien -> loulou
-rw-rw-r-- 0 loulou
-rw-rw-r-- 416 biblo.c
drwxrwxr-x 4096 rep
<b>iam@groot:~/$</b> ./wall-e tests/output.stat tests/ici 
<b>iam@groot:~/$</b> ls tests/
ici  output.stat
<b>iam@groot:~/$</b> ls -l tests/ici
total 8
-rw-r--r-- 1 iam groot  416 mai 10 19:08 biblo.c
lrwxrwxrwx 1 iam groot    6 mai 10 19:08 lien -> loulou
-rw-r--r-- 1 iam groot    0 mai 10 19:08 loulou
drwxr-xr-x 2 iam groot 4096 mai 10 19:08 rep
</pre>
Notez que les droits des fichiers et des répertoires ne sont pas maintenus. Autrement dit, on ignore les permissions dans le fichier `.stat`.


## Avant de commencer

* Cloner (le bouton `fork` en haut à droite) ce dépôt sur le gitlab départemental.
* Le rendre privé : dans `Settings` → `General` → `Visibility` → `Project visibility` → `Private` (n'oubliez pas d'enregistrer).
* Ajouter l'utilisateur `@abdenbi_m` comme mainteneur (oui, j'ai besoin de ce niveau de droits) : `Project information` → `Members` → `Invite member` → `@abdenbi_m`.
* ⚠️ Mal effectuer ces étapes vous expose à des pénalités importantes sans préavis.

## Description de l'outil

```
wall-e FICHIER.stat DIR
```

`wall-e` prend 2 paramètres en entrée,
- `FICHIER.stat` un fichier texte régulier au format `stat`,
- `DIR` le nom d'un répertoire.

Si le répertoire `DIR` existe déjà, `wall-e` va l'utiliser directement. Si non, `wall-e` va créer le répertoire `DIR`.
Par la suite, `wall-e` va lire le contenu du fichier `FICHIER.stat` et va créer son contenu dans le répertoire `DIR`.

<p>

<details>

<summary>Exemple</summary>

<pre>
<b>iam@groot:~/$</b> ls tests/
output.stat
<b>iam@groot:~/$</b> cat tests/output.stat
lrwxrwxrwx 6 lien -> loulou
-rw-rw-r-- 0 loulou
-rw-rw-r-- 416 biblo.c
drwxrwxr-x 4096 rep
<b>iam@groot:~/$</b> ./wall-e tests/output.stat tests/ici 
<b>iam@groot:~/$</b> ls tests/
ici  output.stat
<b>iam@groot:~/$</b> ls -l tests/ici
total 8
-rw-r--r-- 1 iam groot  416 mai 10 19:08 biblo.c
lrwxrwxrwx 1 iam groot    6 mai 10 19:08 lien -> loulou
-rw-r--r-- 1 iam groot    0 mai 10 19:08 loulou
drwxr-xr-x 2 iam groot 4096 mai 10 19:08 rep
</pre>
Notez que les droits des fichiers et des répertoires ne sont pas maintenus. Autrement dit, on ignore les permissions dans le fichier `.stat`.
</details>

</p>

## Format `stat`

Un fichier au format `stat` est une liste de fichiers réguliers, liens symboliques et/ou répertoires, un par ligne.

Chaque ligne est une description d'un fichier régulier, d'un lien symbolique ou d'un répertoire qui contient les informations suivantes (dans l'ordre):

- Le type de fichier linux (un octet) et ces droits (9 octets). Le type pouvant être,
   - `-` pour les fichiers réguliers,
   - `d` pour les répertoires,
   - `l` pour les liens symboliques.
- Suivi d'un espace (` `).
- Un entier qui représente la taille du fichier, du lien symbolique ou du répertoire.
- Suivi d'un espace (` `).
- Une chaîne de caractères représantant le nom du fichier, du lien symbolique ou du répertoire. Dans le cas d'un lien symbolique, le nom du lien est suivi 
     - d'un espace (` `), 
     - de deux caractères `->`,
     - d'un espace (` `),
     - une chaîne de caractères qui indique le chemin vers le fichier lié.

### Fonctionnement

Une fois que le répertoire `DIR` est créé (ou déjà présent), `wall-e` va lire le contenu de `FICHIER.stat` ligne par ligne et va tenter de créer le fichier, le lien symbolique ou le répertoire correspondant. 
Le comportement de `wall-e` va différer selon la ligne lue.
- Dans le cas d'un fichier régulier (type `-`),
   - si le fichier existe déjà dans `DIR` il sera écrasé,
   - le nouveau fichier créer doit avoir les droits `0644` et doit être rempli d'octets pour qu'il est exactement la même taille indiquée dans la ligne qui lui correspond dans le fichier `FICHIER.stat`.
- Dans le cas d'un lien symbolique (type `l`),
   - si un lien symbolique portant le même nom existe (peu importe vers quel fichier il pointe), il devra être supprimé,
   - le nouveau lien symbolique créé va pointer vers le fichier lié indiqué dans la ligne qui lui correspond dans le fichier `FICHIER.stat` (donc la chaîne de caractères qui suivra `->` et ` `).
- Dans le cas d'un répertoire (type `d`),
   - si le répertoire existe déjà dans le répertoire `DIR`, `wall-e` doit s'arrêter immédiatement et retourner `1`,
   - le nouveau répertoire doit être créer avec les droits `0755`

### Traitement des erreurs et valeur de retour

`wall-e` retourne **0** en cas de succès et **1** en cas d'échec. **Aucun message d'erreur ne doit être affiché** et tous les retours d'appels système doivent être traités correctement. Si un appel système échoue, `wall-e` s'arrête immédiatement et retourne la valeur **1**.

## Directives d'implémentation

Vous devez développer le programme en C.
Le fichier source doit s'appeler `wall-e.c` et être à la racine du dépôt.
Vu la taille du projet, tout doit rentrer dans ce seul fichier source.

Pour la réalisation du TP, vous devez respecter les directives suivantes.

### Appels système

**Vous devez utiliser** les appels système suivants,

- `mkdir` pour créer les répertoires.
- `open` pour toute création de fichier régulier et `write` pour le remplir à la taille désirée.
   - Vous utiliserez `close` pour fermer les fichiers que vous aurez ouvret.
- `symlink` pour créer les liens symboliques et `unlink` pour supprimer éventuellement des liens symboliques.
- Lisez bien les cas d'erreurs de ces appels système et vérifier la valeur de `errno` avant d'arrêter `wall-e`, s'il y a lieu.
- Noubliez pas de traiter les erreurs de chaque appel système.

### Précisions

- Vous pouvez supposer que les arguments avec lesquels l'utilitaire `wall-e` sera testé sont valides. C'est à dire,
   - `FICHIER.stat` est chemin vers un fichier qui respecte le format `stat` décrit plus haut. Vous n'avez donc pas à vérifier que le fichier `FICHIER.stat` existe et qu'il respecte le format `stat`.
   - `DIR` est un chemin valide vers un répertoire.
- Mis à part l'utilisation des appels système cités plus haut qui est obligatoire, vous pouvez utiliser n'importe quelle fonction des librairies standard du C (par exemple, la libairie `string.h` pour faciliter le traitement des chaînes de caractères).
   - Notamment, pour lire le fichier `FICHIER.stat`, vous pouver utiliser `fopen` et `getline` par exemple.
- Comme le TP n'est pas si gros (de l'ordre de grandeur d'une centaine de lignes), il est attendu un effort important sur le soin du code et la gestion des cas d'erreurs.

## Acceptation et remise du TP

### Remise

La remise s'effectue simplement en poussant votre code sur la branche `main` (c'est la seule branche qui sera considérée) de votre dépôt gitlab privé.
Seule la dernière version disponible avant le **lundi 30 mai à 23h55** sera considérée pour la correction.

### Intégration continue

Vouz pouvez compiler avec `make` (le `Makefile` est fourni).

Vous pouvez vous familiariser avec le contenu du dépôt, en étudiant chacun des fichiers (`README.md`, `Makefile`, `check.bats`, `.gitlab-ci.yml`, etc.).

⚠️ À priori, il n'y a pas de raison de modifier un autre fichier du dépôt.
Si vous en avez besoin, ou si vous trouvez des bogues ou problèmes dans les autres fichiers, merci de me contacter.

Le système d'intégration continue vérifie votre TP à chaque `push`.
Vous pouvez vérifier localement avec `make check` (l'utilitaire `bats` entre autres est nécessaire).

Les tests fournis ne couvrent que les cas d'utilisation de base, en particulier ceux présentés ici.
Il est **fortement suggéré** d'ajouter vos propres tests dans [local.bats](local.bats) et de les pousser pour que l’intégration continue les prenne en compte.
Ils sont dans un job distincts pour avoir une meilleure vue de l'état du projet.

❤ En cas de problème pour exécuter les tests sur votre machine, merci de 1. lire la documentation présente ici et 2. poser vos questions en classe ou sur [Mattermost](https://mattermost.info.uqam.ca/forum/channels/inf3173).
Attention toutefois à ne pas fuiter de l’information relative à votre solution (conception, morceaux de code, etc.)

### Barème et critères de correction

Le barème utilisé est le suivant

* Seuls les tests qui passent sur l'instance `gitlab.info.uqam.ca` (avec l'intégration continue) seront considérés.
  * 60%: pour le jeu de test public fourni dans le sujet (voir section intégration).
  * 40%: pour un jeu de test privé exécuté lors de la correction. Ces tests pourront être plus gros, difficiles et/ou impliquer des cas limites d'utilisation (afin de vérifier l'exactitude et la robustesse de votre code).
* Des pénalités pour des bogues spécifiques et des défauts dans le code source du programme, ce qui inclut, mais sans s'y limiter l'exactitude, la robustesse, la lisibilité, la simplicité, la conception, les commentaires, etc.
* Note: consultez la section suivante pour des exemples de pénalités et éventuellement des conseils pour les éviter.

## Mentions supplémentaires importantes

⚠️ **Intégrité académique**
Rendre public votre dépôt personnel ou votre code ici ou ailleurs ; ou faire des MR contenant votre code vers ce dépôt principal (ou vers tout autre dépôt accessible) sera considéré comme du **plagiat**.

⚠️ Attention, vérifier **=/=** valider.
Ce n'est pas parce que les tests passent chez vous ou ailleurs ou que vous avez une pastille verte sur gitlab que votre TP est valide et vaut 100%.
Par contre, si des tests échouent quelque part, c'est généralement un bon indicateur de problèmes dans votre code.

⚠️ Si votre programme **ne compile pas** ou **ne passe aucun test public**, une note de **0 sera automatiquement attribuée**, et cela indépendamment de la qualité de code source ou de la quantité de travail mise estimée.
Il est ultimement de votre responsabilité de tester et valider votre programme.

Pour les tests, autant publics que privés, les résultats qui font foi sont ceux sur l'instance `gitlab.info.uqam.ca`. Si un test réussi presque ou de temps en temps, il est considéré comme échoué (sauf rares exceptions).


Quelques exemples de bogues fréquents dans les copies TP de INF3173 qui causent une perte de points, en plus d'être responsable de tests échoués:

* Utilisation de variables ou de mémoire non initialisés (comportement indéterminé).
* Mauvaise gestion de la mémoire dynamique (`free`, `malloc`, `calloc` et compagnie).
* Mauvaise vérification des cas d'erreur des fonctions et appels système (souvent comportement indéterminé si le programme continue comme si de rien n'était)
* Utilisation de valeurs numériques arbitraires (*magic number*) qui cause des comportements erronés si ces valeurs sont dépassées (souvent dans les tailles de tableau).
* Code inutilement compliqué, donc fragile dans des cas plus ou moins limites.


Quelques exemples de pénalités additionnelles:

* Vous faites une MR sur le dépôt public avec votre code privé : à partir de -10%
* Vous demandez à être membre du dépôt public : -5%
* Si vous critiquez à tort l'infrastructure de test : -10%
* Vous modifiez un fichier autre que le fichier source ou `local.bats` (ou en créez un) sans avoir l’autorisation : à partir de -10%
* Votre dépôt n'est pas un `fork` de celui-ci : -100%
* Votre dépôt n'est pas privé : -100%
* L'utilisateur `@abdenbi_m` n'est pas mainteneur : -100%
* Votre dépôt n'est pas hébergé sur le gitlab départemental : -100%
* Vous faites une remise par courriel : -100%
* Vous utilisez « mais chez-moi ça marche » (ou une variante) comme argument : -100%
* Si je trouve des morceaux de votre code sur le net (même si vous en êtes l'auteur) : -100%

