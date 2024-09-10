/*
 * Ecole Polytechnique Montreal - GIGLL
 * Automne 2024
 * SystemCalls - part2.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void part21 ()
{
    // TODO
    const char nullCharacter = '\0';
    const char* outputMessage = "77dbcb01f571f1c32p196c3a7d27f62e\n";
    const ssize_t lengthOfMessage = 33;
    const ssize_t standardOutput = 1;

    for (int i = 0; outputMessage[i] != nullCharacter; i++) {
        printf("%c",outputMessage[i]);
    }

    write(standardOutput,outputMessage,lengthOfMessage);
}

void part22 ()
{
    // TODO
    setvbuf(stdout, NULL, _IONBF, 0);

    const char nullCharacter = '\0';
    const char* outputMessage = "77dbcb01f571f1c32p196c3a7d27f62e\n";
    const ssize_t lengthOfMessage = 33;
    const ssize_t standardOutput = 1;


    printf("%s", outputMessage);

    write(standardOutput, outputMessage, lengthOfMessage);
    
}

int main (int argc, char* argv[])
{
    
    if (argc!=2)
    {   printf("Le programme a un seul paramètre : 1 ou 2\n");
        return 1;
    }
    switch (atoi(argv[1])) {
         case 1:        part21();
                        break;
         case 2:        part22();
                        break;
        default:        printf(" Le paramètre du programme est invalide\n");
                        return 1;
    }

    return 0;
}
