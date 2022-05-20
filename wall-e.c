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
 
const int DIR_MODE1 = 0700;
const int DIR_MODE2 = 0755;

void valider_dir2(char *path,const int dir_mode) {
    struct stat st = {0};
    if (stat(path, &st) == -1) {
        if (mkdir(path, dir_mode) != 0)
            exit(1);
    } else if (dir_mode == DIR_MODE2 && stat(path, &st) == 0)
        exit(1);
}

/*
* Valide le nb arguments recus par le programme. Affiche l'erreur correspndate et arrête le programme s'il y a plus
* qu'un argument valide
*
* @param argc nb d'arguments.
* */
void valider_nb_args(int argc) {
    if (argc != 3)
        exit(1);
}

FILE *ouvrir_fichier(char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
        exit(1);
    return file;
}

int obtenir_taille(const char *ligne) {
    char *taille = strstr(ligne, " ") + 1;
    int i;
    for (i = 0; isdigit(taille[i]); i++);
    char subbuff[i];
    memcpy(subbuff, taille, i);
    return atoi(subbuff);
}

void creer_fregulier(char *ligne, char *argv[]) {
    int taille = (int) obtenir_taille(ligne);
    char *path = creer_path(ligne, argv);
    ecrire_fichier(path, taille);
    free(path);
}

void lire_file(FILE *file, char *argv[]) {
    size_t len = 0;
    char *ligne;
    while ((int) (len = getline(&ligne, &len, file)) > 0) {
        if (ligne[0] == '-')
            creer_fregulier(ligne, argv);
        else if (ligne[0] == 'l')
            creer_lsymbolique();
        else
            creer_repertoire();
    }
}

int main(int argc, char ** argv) {
    valider_nb_args(argc);
    valider_dir(argv[2], DIR_MODE1);
    FILE *file = ouvrir_fichier(argv);
    lire_file(file, argv);
    return 0;
}

