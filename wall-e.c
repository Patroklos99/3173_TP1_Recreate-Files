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


const int DIR_MODE1 = 0700;  //Mode des droits complets pour l'usager seulement.
const int DIR_MODE2 = 0755;  //Mode demandé dans le TP.
const int ARG_1 = 1;  //1 argument passé au programme
const int ARG_2 = 2;  //2 argument passé au programme

/**
 * validation des repertoires, arrete le programme lorsque validation correspondante precise.
 *
 * @param chemin pointeur vers le chemin a ecrire
 * @param argv pointeur vers les arguments.
 */
void valider_dir(char *chemin, const int dir_mode) {
    struct stat st = {0};
    if (stat(chemin, &st) == -1) {
        if (mkdir(chemin, dir_mode) != 0)
            exit(1);
    } else if (dir_mode == DIR_MODE2 && stat(chemin, &st) == 0)
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
 * Obtient la taille dun fichier. Regulier, repertoire ou symbolique
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 */
int obtenir_taille(const char *ligne) {
    char *taille = strstr(ligne, " ") + 1; //Deplacer le pointeur de +1 charactere après l'espace.
    int i;
    for (i = 0; isdigit(taille[i]); i++);
    char sub_tampon[i];
    memcpy(sub_tampon, taille, i);
    return atoi(sub_tampon);
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
    char mot [strlen(strstr(ligne, ">") + 2)]; //Deplacer le pointeur de +2 characteres après le charctere > vers le 1er charactere du mot suivant.
    memcpy(mot, strstr(ligne, ">") + 2, strlen(strstr(ligne, ">") + 2));
    if (symlink(mot, chemin))
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
}

/**
 * Cree un repertoire
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 * @param argv pointeur contenant les arguments.
 */
void creer_repertoire(char *ligne, char *argv[]) {
    int taille = (int) obtenir_taille(ligne);
    char *chemin = creer_path(ligne, argv);
    valider_dir(chemin, DIR_MODE2);
    ecrire_fichier(chemin, taille);
    free(chemin);
}

/**
 * Cree un lien symbolique
 *
 * @param ligne pointeur vers la ligne lu a manipuler.
 * @param argv pointeur contenant les arguments.
 */
void creer_lsymbolique(char *ligne, char *argv[]) {
    char ligne_copie [strlen(ligne)];
    strcpy(ligne_copie, ligne);
    char *chemin = creer_path(ligne_copie, argv);
    struct stat tampon;
    if (lstat(chemin, &tampon) == 0)
        unlink(chemin);
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
    char *chemin = creer_path(ligne, argv);
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
    char *ligne;
    while ((int) (len = getline(&ligne, &len, file)) > 0) {
        if (ligne[0] == '-')
            creer_fregulier(ligne, argv);
        else if (ligne[0] == 'l')
            creer_lsymbolique(ligne, argv);
        else
            creer_repertoire(ligne, argv);
    }
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
    return 0;
}

