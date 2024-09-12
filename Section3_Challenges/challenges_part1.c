/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/
#include "challenges_part1.h"


int ExpolrerRepertoire(char *chemin){
    DIR *repertoire = opendir(chemin);

    int cntFichiers = 0;

    dirent *entre;

    while((entre = readdir(repertoire)) != NULL){
        if (strcmp(entre->d_name, ".") == 0 || strcmp(entre->d_name, "..") == 0) {
            continue;
        }

        
        char cheminComplet[MAX_PATH_LENGTH];
        snprintf(cheminComplet, sizeof(cheminComplet), "%s/%s", chemin, entre->d_name);

        if (entre->d_type == DT_REG) {
            cntFichiers++;
        } else {
            if(fork() == 0){
                int count = ExpolrerRepertoire(cheminComplet);
                _exit(count);
            }
        }

    }

    closedir(repertoire);

    int status;

    while (wait(&status) > 0) {
        if (WIFEXITED(status)) {
            cntFichiers += WEXITSTATUS(status);
        }
    }

    return cntFichiers;
}

int main(int argc, char*argv[])
{
    int somme = ExpolrerRepertoire("root");

    printf("Nombre total de fichiers texte: %d\n", somme);

    return 0;
}



