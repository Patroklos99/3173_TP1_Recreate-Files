/*  INF3173 - TP1
 *  Session : été 2022
 *
 *  IDENTIFICATION.
 *
 *      Nom : Salcedo
 *      Prénom : Renzo A.
 *      Code permanent : SALR02089408
 *      Groupe : 40
 */

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

const int DIR_MODE1 = 0700;  //Mode des droits complets pour l'usager seulement.
const int DIR_MODE2 = 0755;  //Mode demandé dans le TP.
const int ARG_1 = 1;  //1 argument passé au programme
const int ARG_2 = 2;  //2 argument passé au programme
const int LONGUEUR_MAX = 255; //longueur max permise pour fichier,lien, repertoire par linux.

/**
 * validation des repertoires, arrete le programme lorsque validation correspondante precise.
 *
 * @param chemin pointeur vers le chemin a ecrire
 * @param dir_mode mode pour valider les conditions if respectives.
 * @param file pointeur vers fichier a liberer.
 */
void valider_dir2(char *chemin, const int dir_mode, char *ligne, FILE *file) {
    if (dir_mode == DIR_MODE2 && mkdir(chemin, dir_mode) != 0) {
        free(ligne);
        free(chemin);
        free(file);
        exit(1);
    }
}

/**
 * validation des repertoires, arrete le programme lorsque validation correspondante precise.
 *
 * @param chemin pointeur vers le chemin a ecrire
 * @param dir_mode mode pour valider les conditions if respectives.
 */
void valider_dir(char *chemin, const int dir_mode) {
    if (dir_mode == DIR_MODE1 && mkdir(chemin, dir_mode) == -1 && errno != EEXIST)
        exit(1);
}

/**
* Valide le nb arguments recus par le programme. Arrête le programme s'il y a plus que 2 args.
*
* @param argc nb d'arguments.
* */
void valider_nb_args(int argc) {
    if (argc != 3)
        exit(1);
}

/**
 * Ouvre le fichier passer en argument pour lecture.
 *
 * @param argv pointeur vers les arguments.
 * */
FILE *ouvrir_fichier(char *argv[]) {
    FILE *file = fopen(argv[ARG_1], "r");
    if (file == NULL)
        exit(1);
    return file;
}

/**
 * Valide lutilisation de la function strtol.
 *
 * @param i taille en type entier.
 */
int valider_strtol(char *sub_tampon) {
    char *ptr = NULL;
    long numero = strtol(sub_tampon, &ptr, 10);
    if (numero == LONG_MIN || numero == LONG_MAX)
        exit(1);
    else
        return (int) numero;
}

/**
 * Obtient la taille dun fichier. Regulier, repertoire ou symbolique
 *
 * @param i taille en type entier.
 */
int obtenir_taille(const char *ligne) {
    char *taille = strstr(ligne, " ") + 1; //Deplacer le pointeur de +1 charactere après l'espace.
    int i = 0;
    for (; isdigit(taille[i]) != 0; i++);
    char sub_tampon[i];
    memcpy(sub_tampon, taille, i);
    return valider_strtol(sub_tampon);
}

/**
 * Verifie la longueur du fichier,repertoire ou lien symbolique. Arrete le programme
 * si plus grand que longueur max permise dans linux 255.
 *
 * @param mot mot a verifier.
 */
void verifierlongueur(char *mot) {
    if (strlen(mot) > LONGUEUR_MAX)
        exit(1);
}

/**
 * Cree un string dun chemin desiré
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 * @param argv pointeur vers les arguments.
 */
char *creer_path(char *ligne, char *argv[]) {
    char *mot = strtok(ligne, " \n\0");
    for (int i = 0; i < 2; ++i)
        mot = strtok(NULL, " \n\0");
    verifierlongueur(mot);
    char chemin[strlen(argv[ARG_2]) + strlen(mot) + 2]; //+2 pour les characteres "/" et \0 (fin de ligne)
    snprintf(chemin, sizeof(chemin), "%s%s%s", argv[2], "/", mot);
    return strdup(chemin);
}

/**
 * Ecrit un lien symbolique
 *
 * @param chemin pointeur vers le chemin a ecrire
 * @param ligne pointeur vers la ligne lu a manipuler.
 */
void ecrire_fichier_symbol(char *chemin, char *ligne) {
    ligne[strlen(ligne) - 1] = '\0';
    char mot[strlen(strstr(ligne, ">") +
                    2)]; //Deplacer le pointeur de +2 characteres après le charctere > vers le 1er charactere du mot suivant.
    memcpy(mot, strstr(ligne, ">") + 2, strlen(strstr(ligne, ">") + 2));
    if (symlink(mot, chemin) != 0)
        exit(1);
}

/**
 * Ecrit un fichier.
 *
 * @param chemin pointeur vers le chemin a ecrire
 * @param taille du fichier a ecrire.
 */
void ecrire_fichier(char *chemin, int taille) {
    int fd = open(chemin, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, chemin, (size_t) taille);
    if (close(fd) == -1)
        exit(1);
}

/**
 * Cree un repertoire
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 * @param argv pointeur contenant les arguments.
 */
void creer_repertoire(char *ligne, char *argv[], FILE *file) {
    char *chemin = NULL;
    chemin = creer_path(ligne, argv);
    valider_dir2(chemin, DIR_MODE2, ligne, file);
    free(chemin);
}

/**
 * Cree un lien symbolique
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 * @param argv pointeur contenant les arguments.
 */
void creer_lsymbolique(char *ligne, char *argv[]) {
    char ligne_copie[strlen(ligne)];
    strcpy(ligne_copie, ligne);
    char *chemin = NULL;
    chemin = creer_path(ligne_copie, argv);
    struct stat tampon;
    if (lstat(chemin, &tampon) == 0)
        if (unlink(chemin) == -1)
            exit(1);
    ecrire_fichier_symbol(chemin, ligne);
    free(chemin);
}

/**
 * Cree un fichier regulier.
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 * @param argv pointeur contenant les arguments.
 */
void creer_fregulier(char *ligne, char *argv[]) {
    int taille = (int) obtenir_taille(ligne);
    char *chemin = NULL;
    chemin = creer_path(ligne, argv);
    ecrire_fichier(chemin, taille);
    free(chemin);
}

/**
 * Lecture du fichier.
 *
 * @param file pointeur vers le fichier a lire
 * @param argv pointeur contenant les arguments.
 */
void lire_file(FILE *file, char *argv[]) {
    size_t len = 0;
    char *ligne = NULL;
    while ((int) (len = getline(&ligne, &len, file)) > 0) {
        if (ligne[0] == '-')
            creer_fregulier(ligne, argv);
        else if (ligne[0] == 'l')
            creer_lsymbolique(ligne, argv);
        else if (ligne[0] == 'd')
            creer_repertoire(ligne, argv, file);
        else {
            free(ligne);
            exit(1);
        }
    }
    free(ligne);
}

/**
 * Méthode main, lance tous les sous-methodes correspondate.
 *
 *@param argc nb d'arguments.
 *@param argv pointeur contenant les arguments.
*/
int main(int argc, char *argv[]) {
    valider_nb_args(argc);
    valider_dir(argv[ARG_2], DIR_MODE1);
    FILE *file = ouvrir_fichier(argv);
    lire_file(file, argv);
    fclose(file);
    return 0;
}

