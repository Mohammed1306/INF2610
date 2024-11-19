#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int fd[2];

    pipe(fd);

    if (!fork())
    { // le fils 1 ecris dans le tube
        close(fd[1]);
        char *string[6];
        read(fd[0], string, 6);
        exit(0);
    }

    if (!fork())
    { // le fils 2 execute ls
        close(fd[1]);
        close(fd[0]);
        execlp("ls", "ls", "-l", NULL);
        exit(0);
    }

    /*
    Cette ligne cause l'interblocage.
    Conditions:
        Exclusion mutuelle: L'accés en écriture au tube du père est alloué au père 
        Détention et attente: Le premier fils peut attendre la fermeture de l'accés au tube en écriture de son père
        Pas de réquisition: Le processus parent est le seul qui peut fermer son accés en écriture au tube
        Attente circulaire: Le premier fils attend la fermeture de l'accés au tube en écriture de son père qui lui même attend la fin de son fils
    Interblocage vu en cours: l'appel système read bloquant


    while (wait(NULL) > 0);
    */
    char string[6] = "Salut!";
    write(fd[1], &string, strlen(string));
    printf("Process completed!\n");
    close(fd[0]);
    close(fd[1]);
    // Correction:
    while (wait(NULL) > 0);
    /*
    Explication: Le processus père ferme ses accés au tube, et surtout son accés en écriture, avant d'attendre
    la fin de ses fils. Cela permet donc de débloquer le premier fils ce qui débloque le wait du père.
    */
    return 0;
}