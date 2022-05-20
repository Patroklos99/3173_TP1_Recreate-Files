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

void ecrire_fichier(char *path, int taille) {
    int fd = open(path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    write(fd, path, (size_t) taille);
}

char *creer_path(char *ligne, char *argv[]) {
    char *word = strtok(ligne, " \n\0");
    for (int i = 0; i < 2; ++i)
        word = strtok(NULL, " \n\0");
    char path[strlen(argv[2]) + strlen(word) + 2];
    snprintf(path, sizeof(path), "%s%s%s", argv[2], "/", word);
    return strdup(path);
}

int obtenir_taille(const char *ligne) {
    char *taille = strstr(ligne, " ") + 1;
    int i;
    for (i = 0; isdigit(taille[i]); i++);
    char subbuff[i];
    memcpy(subbuff, taille, i);
    return atoi(subbuff);
}

void creer_repertoire(char *ligne, char *argv[]) {
    int taille = (int) obtenir_taille(ligne);
    char *path = creer_path(ligne, argv);
    valider_dir(path, DIR_MODE2);
    char path2[strlen(path) + 6];
    strcpy(path2, path);
    strcat(path2, "/test");
    ecrire_fichier(path2, taille);
    free(path);
}

void ecrire_fichier_symbol(char *path, char *ligne) {
    ligne[strlen(ligne) - 1] = '\0';
    char word [strlen(strstr(ligne, ">") + 2)];
    memcpy(word, strstr(ligne, ">") + 2, strlen(strstr(ligne, ">") + 2));
    if (symlink(word, path))
        exit(1);
}

void creer_lsymbolique(char *ligne, char *argv[]) {
    char teste [strlen(ligne)];
    strcpy(teste, ligne);
    char *path = creer_path(teste, argv);
    struct stat buf;
    if (lstat(path, &buf) == 0)
        unlink(path);
    ecrire_fichier_symbol(path, ligne);
    free(path);
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

