/*
 * Ecole Polytechnique Montreal - GIGL
 * Automne 2024
 * Processes - part2.c
 *
 * Mohammed Abdellatif Kallel 2292761, Joey Hasrouny 2185301
*/

#include "./libprocesslab/libprocesslab.h"

#define m 1000000

// question2 calcule la somme des m premiers nombres naturels 1+2+...+m

// nb est le nombre de threads qui vont contribuer au calcul
#define nb 4

//tableau somme est utilisé pour le calcul des sommes patielles par les threads
long somme[nb];


// fonction exécutée par chaque thread créé
void* contribution(void*p)
{
  int no = *((int*)p);
  int start = (no * m / nb) + 1;
  int end = (no + 1) * m / nb;
  long result = 0;
  for(int i = start; i <= end; i++){
    result += i;
  }

  somme[no] = result;

  return NULL;
}


void question2( )
{
  int thread_ids[nb];
  pthread_t threads[nb]; 
  int i;
  for (i = 0; i < nb; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, contribution, (void *)&thread_ids[i]) != 0) {
            perror("Erreur lors de la création du thread");
            exit(EXIT_FAILURE);
        }
    }


    for (i = 0; i < nb; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Erreur lors de la terminaison du thread");
            exit(EXIT_FAILURE);
        }
    }

    long actualResult = 0;

    for(i = 0; i < nb; i++){
      actualResult += somme[i];
    }

    long expectedResult =(long) m * (m+1) / 2;



    printf("Résultat obtenu: %ld \nRésultat attendu: %ld\n", actualResult, expectedResult);

    
}

