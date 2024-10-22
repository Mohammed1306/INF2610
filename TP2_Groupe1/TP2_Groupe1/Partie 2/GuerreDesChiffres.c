#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>

bool flag_de_fin = false;
int produits = 0;
int consommes = 0;

typedef struct
{
    int *tampon;
    int taille;
    int ip;
    int ic;
    sem_t libre;  // Ne produire que s'il y a une case vide à produire
    sem_t occupe; // Ne consommer que s'il y a un produits à consommer
    sem_t mutex;  // Donner un accès exclusif à ip et ic
} SharedBuffer;

void handler(int signum)
{
    flag_de_fin = true;
}

void *producteur(void *arg)
{
    SharedBuffer *buffer = (SharedBuffer *)arg;

    srand(time(NULL));
    int somme = 0;
    while (1)
    {
        int item = (rand()%9) + 1;

        somme += item;

        sem_wait(&buffer->libre);
        sem_wait(&buffer->mutex);

        buffer->tampon[buffer->ip] = item;
        buffer->ip = (buffer->ip + 1) % buffer->taille;
        produits++;

        sem_post(&buffer->mutex);
        sem_post(&buffer->occupe);

        if (flag_de_fin)
        {
            pthread_exit((void *)(intptr_t)somme);
        }
    }

    return NULL;
}

void *consommateur(void *arg)
{
    SharedBuffer *buffer = (SharedBuffer *)arg;
    int somme = 0;

    while (1)
    {
        sem_wait(&buffer->occupe);
        sem_wait(&buffer->mutex);

        int item = buffer->tampon[buffer->ic];
        somme += item;
        buffer->ic = (buffer->ic + 1) % buffer->taille;
        consommes++;

        sem_post(&buffer->mutex);
        sem_post(&buffer->libre);

        if (item == 0)
        {
            pthread_exit((void *)(intptr_t)somme);
        }
    }

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <nb_producteurs> <nb_consommateurs> <taille_tampon>\n", argv[0]);
        return 1;
    }

    signal(SIGALRM, handler);

    int nbProducteurs = atoi(argv[1]);
    int nbConsommateurs = atoi(argv[2]);
    int tailleTampon = atoi(argv[3]);

    SharedBuffer buffer;
    buffer.taille = tailleTampon;
    buffer.tampon = malloc(tailleTampon * sizeof(int));
    buffer.ic = 0;
    buffer.ip = 0;

    sem_init(&buffer.libre, 0, tailleTampon);
    sem_init(&buffer.occupe, 0, 0);
    sem_init(&buffer.mutex, 0, 1);

    pthread_t producteurs[nbProducteurs];
    for (int i = 0; i < nbProducteurs; i++)
    {
        pthread_create(&producteurs[i], NULL, producteur, (void *)&buffer);
    }

    pthread_t consommateurs[nbConsommateurs];
    for (int i = 0; i < nbConsommateurs; i++)
    {
        pthread_create(&consommateurs[i], NULL, consommateur, (void *)&buffer);
    }

    alarm(1);

    int *resultatProduits = malloc(sizeof(int));
    int *resultatConsommes = malloc(sizeof(int));
    int sommeProduits = 0;
    int sommeConsommations = 0;

    for (int i = 0; i < nbProducteurs; i++)
    {
        pthread_join(producteurs[i], (void **)&resultatProduits);
        sommeProduits += (int)(intptr_t)resultatProduits;
    }

    for (int i = 0; i < nbConsommateurs; i++)
    {

        sem_wait(&buffer.libre);
        sem_wait(&buffer.mutex);

        buffer.tampon[buffer.ip] = 0;
        buffer.ip = (buffer.ip + 1) % buffer.taille;
        produits++;

        sem_post(&buffer.mutex);
        sem_post(&buffer.occupe);
    }

    for (int i = 0; i < nbConsommateurs; i++)
    {
        pthread_join(consommateurs[i], (void **)&resultatConsommes);
        sommeConsommations += (int)(intptr_t)resultatConsommes;
    }

    printf("Somme des porduits: %d Somme des consommations: %d \n", sommeProduits, sommeConsommations);
    printf("Nombre des porduits: %d Nombre des consommations: %d \n", produits, consommes);

    free(buffer.tampon);
    sem_destroy(&buffer.libre);
    sem_destroy(&buffer.occupe);
    sem_destroy(&buffer.mutex);

    return 0;
}
