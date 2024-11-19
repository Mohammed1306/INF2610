#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define ENTRY_CODE 7
#define N_THREADS 5

sem_t sem_critical;

int try_lock(int key)
{
    if (key == ENTRY_CODE)
    {
        printf("Door unlocked\n");
        return 1;
    }
    sleep(1);
    return 0;
}

void *door(void *args)
{
    while (1)
    {
        sem_wait(&sem_critical);
        int key = rand() % 30;
        /*
        Cette section cause l'interblocage
        Conditions:
            Exclusion mutuelle: sem_critical est alloué à un seul thread en particulier
            Détention et attente: Les autre threads peuvent attendre la libération de sem_critical
            Pas de réquisition: sem_critical ne peut être libéré que par le thread qui le possède
            Attente circulaire: Tous les autres threads attendent que sem_critical se libère alors que le thread qui est supposé le faire de termine avant de le libérer


        if(try_lock(key))

            pthread_exit(NULL);
        sem_post(&sem_critical);
        */

        // Correction:
        if (try_lock(key))
        {
            sem_post(&sem_critical);
            pthread_exit(NULL);
        }
        sem_post(&sem_critical);
        /*
        Explication: Puisque chaque thread libère le mutex avant de se finir, tous les threads
        pourront y accéder.
        */
    }
}
int main()
{

    sem_init(&sem_critical, 0, 1);
    pthread_t threads[N_THREADS];

    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_create(&threads[i], NULL, door, NULL);
    }

    for (int i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}