#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_THREADS 5
#define MAX_ROUNDS 3

// Création du mutex
pthread_mutex_t mutex;

pthread_mutex_t locks[NUM_THREADS];

void *cons(void *arg)
{
    int id = *(int *)arg;
    int rounds = 0;

    while (rounds < MAX_ROUNDS)
    {
        sleep(1);
        int left = id;
        int right = (id + 1) % NUM_THREADS;

        /*
        Cette section cause l'interblocage.
        Conditions:
            Exclusion mutuelle: Le verrou right d'un thread est alloué à un autre thread
            Détention et attente: Tous les threads qui possèdent leurs verrous left peuvent demander leurs verrous right
            Pas de réquisition: Le verrou left de chaque threads ne peut être libéré que par le thread en question
            Attente circulaire: Chaque threadM attend la libération du verrou left du threadN où threadM.right == threadN.left ce qui va créer une boucle d'attente
        Interblocage vu en cours: problème des philosophes


        pthread_mutex_lock(&locks[left]);
        sleep(1);
        pthread_mutex_lock(&locks[right]);
        */

        // Correction:
        pthread_mutex_lock(&mutex);

        pthread_mutex_lock(&locks[left]);
        sleep(1);
        pthread_mutex_lock(&locks[right]);

        sleep(1);
        rounds++;

        pthread_mutex_unlock(&locks[left]);
        pthread_mutex_unlock(&locks[right]);

        pthread_mutex_unlock(&mutex);

        /*
        Explication: On crée un mutex qui servira à donner un accés exlclusif au threads pour prendre les verrous.
        Ceci permettera à chaque threads de prendre les deux verrous dont il a besoin au même temps ce qui résoud
        L'interblocage.
        */
    }

    return NULL;
}

int main()
{
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    // Initialisation du mutex
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_mutex_init(&locks[i], NULL);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, cons, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}