/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * SystemCalls - part1.c
 *
 * Mohammed Abdellatif Kallel 2292761, Joey Hasrouny 2185301
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>


int main () {
    int fp = open("./systemcalls_output2.txt", O_TRUNC | O_WRONLY);

    if(fp < 0){
        perror("Appel système open a échoué");
        return 1;
    }

    printf("Saisissez votre texte suivi de 2* CTRL-D :\n");

    char buffer;

    while(read(0, &buffer, 1) > 0){
        write(fp, &buffer, 1);
    }

    close(fp);


    return 0;
}
