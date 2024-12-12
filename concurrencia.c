#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#define NUM_THREADS      10
#define MAX_COUNT 1000000

//Variable global mutex para controlar acceso a sección crítica
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  //macro de inicialización

//La función que ejecutará cada thread
void *worker(void *ptr) {
    int aux;
    for (size_t i = 0; i < MAX_COUNT; i++)
    {
        pthread_mutex_lock(&mutex);
        aux = my_stack_pop();
        pthread_mutex_unlock(&mutex);
        aux = aux+1;
        pthread_mutex_lock(&mutex);
        my_stack_push();
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);

}

int main (int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];

    for(i=0; i<NUM_THREADS; i++){
        pthread_create(&threads[i], NULL, worker, NULL); // 1st arg es ID de thread y 3rd es la función que ejecuta el thread.
    }

    for(i=0; i<NUM_THREADS; i++){ //El main thread espera hasta que acaben todos los threads
        pthread_join(threads[i], NULL);
    }

    my:stack_write(); //Guarda la pila modificada en un fichero
    my_stack_purge(); //Eliminar 
    pthread_exit(NULL);
    return 0;
}
