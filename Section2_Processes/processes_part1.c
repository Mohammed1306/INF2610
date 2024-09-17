/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Processes - part1.c
 *
 * Mohammed Abdellatif Kallel 2292761, Joey Hasrouny 2185301
*/

#include "libprocesslab/libprocesslab.h"


void question1()
{
    int status;

    registerProc(getpid(), getppid(), 0, 1);

    if(fork() == 0){
        registerProc(getpid(), getppid(), 1, 1);

        if(fork() == 0){
            registerProc(getpid(), getppid(), 2, 1);
            _exit(0);
        }   
        if(fork() == 0){
            registerProc(getpid(), getppid(), 2, 2);
            _exit(0);
        }

        int count = 0;

        while(wait(NULL) > 0){
            count++;
        }
        _exit(count);
    }


    if(fork() == 0){
        registerProc(getpid(), getppid(), 1, 2);

        if(fork() == 0){
            registerProc(getpid(), getppid(), 2, 3);
            _exit(0);
        } 

        int count = 0;

        while(wait(NULL) > 0){
            count++;
        }
        _exit(count);
    }

    if(fork() == 0){
        registerProc(getpid(), getppid(), 1, 3);

        if(fork() == 0){
            registerProc(getpid(), getppid(), 2, 4);
            _exit(0);
        } 
        if(fork() == 0){
            registerProc(getpid(), getppid(), 2, 5);
            _exit(0);
        } 

        if(fork() == 0){
            registerProc(getpid(), getppid(), 2, 6);
            _exit(0);
        } 

        int count = 0;

        while(wait(NULL) > 0){
            count++;
        }
        _exit(count);

    }

    int somme = 0;

    while(wait(&status) > 0){
        somme += WEXITSTATUS(status) + 1;
    }

    printf("La somme de tous les enfants du lvl0 est %d\n", somme);

    printProcRegistrations();

    execl("/usr/bin/ls", "ls", "-l", NULL);

    _exit(0);
}

