#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define N_THREADS 5

int flag = 0;

void* function_that_does_something(void* arg) {
    while (1) {
        if (flag)
            break;
    }
    pthread_exit(NULL);
}

void changeFlag(){
    /*
    Ceci est une autre situation où un blocage (et non un interblocage) pourrait se produire.
    Un nombre modulo 10 ne donnera jamais 10 et le flag ne changera jamais. Il faut changer le
    10 en une valeur entre 0 et 9 inclusivement. De plus, si la condition est vrai, le code aura
    9 chances sur 10 de rouler à l'infini.
    Pour corriger le code, on pourrait enlever la ligne if (rand() % 10 == 10)
    */
    if (rand() % 10 == 10)
        flag = 1;
    printf("Signal called\n");
}

int main() {
    pthread_t threads[N_THREADS];
 
    signal(SIGALRM, &changeFlag);      

    for(int i = 0; i < N_THREADS; i++){
        pthread_create(&threads[i], NULL, function_that_does_something, NULL);
    }

    alarm(1);

    for(int i = 0; i < N_THREADS; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}