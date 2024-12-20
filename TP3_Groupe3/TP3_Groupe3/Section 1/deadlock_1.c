#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define N_THREADS 1
#define N_THREADS_2 10

int buffer[BUFFER_SIZE];
int ip;
int ic;

sem_t sem_initial;
sem_t sem_busy;
sem_t sem_critical;

int flag = 0;

void *producer(void *arg)
{
    while (1)
    {
        sem_wait(&sem_initial);
        sem_wait(&sem_critical);
        buffer[ip] = rand() % 9 + 1;
        ip = (ip + 1) % BUFFER_SIZE;
        sem_post(&sem_critical);
        sem_post(&sem_busy);
        if (flag)
            break;
    }
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    while (1)
    {
        sem_wait(&sem_busy);
        sem_wait(&sem_critical);
        int x = buffer[ic];
        ic = (ic + 1) % BUFFER_SIZE;
        sem_post(&sem_critical);
        sem_post(&sem_initial);
        if (x == 0)
        {
            break;
        }
    }
    pthread_exit(NULL);
}

void changeFlag()
{
    flag = 1;
    printf("Flag changed\n");
}

int main()
{
    pthread_t prod_thread[N_THREADS], cons_thread[N_THREADS_2];

    sem_init(&sem_initial, 0, BUFFER_SIZE);
    sem_init(&sem_busy, 0, 0);
    sem_init(&sem_critical, 0, 1);
    signal(SIGALRM, &changeFlag);

    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_create(&prod_thread[i], NULL, producer, NULL);
    }

    for (int i = 0; i < N_THREADS_2; i++)
    {
        pthread_create(&cons_thread[i], NULL, consumer, NULL);
    }

    alarm(1);

    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(prod_thread[i], NULL);
    }

    /*
    Cette section cause l'interblocage.
    Conditions:
        Exclusion mutuelle: sem_busy est disponible
        Détention et attente: Les consommateurs peuvent demander à sem_busy
        Pas de réquisition: Le processus parent est le seul qui peut libérer sem_busy
        Attente circulaire: Les consommateurs attendent la libération de sem_busy alors que le parent ne le fait jamais
    Interblocage vu en cours: problème du producteur consommateur.


    for(int i = 0; i < N_THREADS_2; i++){
        buffer[ip] = 0;
        ip = (ip + 1) % BUFFER_SIZE;
    }*/

    // Correction:
    for (int i = 0; i < N_THREADS_2; i++)
    {
        sem_wait(&sem_initial);
        sem_wait(&sem_critical);
        buffer[ip] = 0;
        ip = (ip + 1) % BUFFER_SIZE;
        sem_post(&sem_critical);
        sem_post(&sem_busy);
    }
    /*
    Explication: Si on veut produire des zéros, il faut se comporter comme un producteur
    même si on est le processus principal. Ce-dernier va donc informer les consomateurs
    qu'il y a des produits à consomer en libérant un jeton du sem_busy.
    */

    for (int i = 0; i < N_THREADS_2; i++)
    {
        pthread_join(cons_thread[i], NULL);
    }

    sem_destroy(&sem_initial);
    sem_destroy(&sem_busy);
    sem_destroy(&sem_critical);

    return 0;
}