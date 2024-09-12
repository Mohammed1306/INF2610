/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Challenges - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/
#include "challenges_part1.h"

int nextRepId = 1;

typedef struct {
    int dirId;
    int parentId;
    char chemin[MAX_PATH_LENGTH];
    char fichiersTexte[MAX_PATH_LENGTH * 10];
} InfoRepertoire;

void WriteDirectoryInfo(InfoRepertoire *infoRep) {
    FILE *fp = fopen("challenges_output.txt", "a");
    if (!fp) {
        perror("fopen");
        exit(1);
    }
    fprintf(fp, "L'emplacement du repertoire est: %s\n", infoRep->chemin);
    fprintf(fp, "L'id de ce repertoire est: %d\n", infoRep->dirId);
    fprintf(fp, "L'id du parent de ce repertoire est: %d\n", infoRep->parentId);
    fprintf(fp, "Les fichiers dans ce repertoire sont:\n    %s\n", infoRep->fichiersTexte);

    fclose(fp);
}

int ExpolrerRepertoire(char *chemin, int parentId){
    DIR *repertoire = opendir(chemin);
    int cntFichiers = 0;

    InfoRepertoire infoRep;
    infoRep.dirId = nextRepId++;
    infoRep.parentId = parentId;
    /*Section réalisée à l'aide de ChatGpt : début*/
    strncpy(infoRep.chemin, chemin, sizeof(infoRep.chemin) - 1);
    infoRep.chemin[sizeof(infoRep.chemin) - 1] = '\0';
    /*Section réalisée à l'aide de ChatGpt : fin*/
    infoRep.fichiersTexte[0] = '\0';

    char buffer[MAX_PATH_LENGTH];
    dirent *entre;

    while((entre = readdir(repertoire)) != NULL){
        if (strcmp(entre->d_name, ".") == 0 || strcmp(entre->d_name, "..") == 0) {
            continue;
        }

        
        char cheminComplet[MAX_PATH_LENGTH];
        snprintf(cheminComplet, sizeof(cheminComplet), "%s/%s", chemin, entre->d_name);

        if (entre->d_type == DT_REG) {
            /*Section réalisée à l'aide de ChatGpt : début*/
            snprintf(buffer, sizeof(buffer), "%s\n    ", entre->d_name);
            if (strlen(infoRep.fichiersTexte) + strlen(buffer) < sizeof(infoRep.fichiersTexte)) {
                strcat(infoRep.fichiersTexte, buffer);
            }
            /*Section réalisée à l'aide de ChatGpt : fin*/
            cntFichiers++;
        } else {
            if(fork() == 0){
                int count = ExpolrerRepertoire(cheminComplet, infoRep.dirId);
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

    WriteDirectoryInfo(&infoRep);

    return cntFichiers;
}

int main(int argc, char*argv[])
{
    unlink("challenges_output.txt");

    int somme = ExpolrerRepertoire("root", 0);

    printf("Nombre total de fichiers texte: %d\n", somme);

    return 0;
}



