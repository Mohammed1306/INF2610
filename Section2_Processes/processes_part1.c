/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Processes - part1.c
 *
 * Ajoutez vos noms, prénoms et matricules
*/

#include "libprocesslab/libprocesslab.h"


void question1()
{
    pid_t lvl11, lvl12, lvl13, lvl21, lvl22, lvl23, lvl24, lvl25, lvl26;


    lvl11 = fork();
    if(lvl11 == 0){

        lvl21 = fork();
        if(lvl21 == 0)
            _exit(0);

        lvl22= fork();
        if(lvl22 == 0)
            _exit(0);

        wait(NULL);
        wait(NULL);

    }


    lvl12 = fork();
    if(lvl12 == 0){

        lvl23 = fork();
        if(lvl23 == 0)
            _exit(0);

        wait(NULL);

    }

    lvl13 = fork();
    if(lvl13 == 0){

        lvl24 = fork();
        if(lvl24 == 0)
            _exit(0);

        lvl25 = fork();
        if(lvl25 == 0)
            _exit(0);

        lvl26 = fork();
        if(lvl26 == 0)
            _exit(0);

        wait(NULL);
        wait(NULL);
        wait(NULL);

    }


    wait(NULL);
    wait(NULL);
    wait(NULL);
}

