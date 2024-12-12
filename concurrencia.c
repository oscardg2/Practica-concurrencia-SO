#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define NUM_THREADS 10
#define MAX_COUNT 1000000

// Variable global mutex para controlar acceso a sección crítica
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // macro de inicialización

// La función que ejecutará cada thread
void *worker(void *ptr)
{
    int my_id = getpid(); 
    printf("Thread ID: %d ejecutando\n", my_id);

    int aux;
    for (size_t i = 0; i < MAX_COUNT; i++)
    {
        pthread_mutex_lock(&mutex);
        aux = my_stack_pop();
        printf("Thread ID: %d ejecutando pop\n", my_id); //Mostrar el mensaje cuando está haciendo pruebas pequeñas
        pthread_mutex_unlock(&mutex);
        aux = aux + 1;
        pthread_mutex_lock(&mutex);
        printf("Thread ID: %d ejecutando pop\n", my_id); //Mostrar el mensaje cuando está haciendo pruebas pequeñas
        my_stack_push();
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS]; //Los id de los threads

    for (size_t i = 0; i < NUM_THREADS; i++) //Bucle para crear los threads, 1st arg es el id y el 3rd es la función que ejecutará el thread
    {
        if (pthread_create(&threads[i], NULL, worker, NULL) != 0) 
        {
            perror("Error creando el hilo");
            exit(EXIT_FAILURE);
        }
    }

    for (size_t i = 0; i < NUM_THREADS; i++) //Bucle para que el main thread quede bloqueado hasta que acaben todos los threads. 
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error uniendo el hilo");
            exit(EXIT_FAILURE);
        }
    }

    my_stack_write(); // Guarda la pila modificada en un fichero
    my_stack_purge(); // Eliminar
    pthread_exit(NULL);
    return 0;
    
}
