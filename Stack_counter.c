#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "my_lib.h" // Declaraciones del gestor de pila

#define NUM_THREADS 10
#define MAX_COUNT 1000000

// Mutex global para proteger el acceso concurrente a la pila
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Estructura para pasar parámetros a los hilos
struct thread_data {
    struct my_stack *stack;
};

// Función que ejecutarán los hilos
void *worker(void *ptr) {
    struct thread_data *data = (struct thread_data *)ptr;
    struct my_stack *stack = data->stack;

    for (size_t i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        int *value = (int *)my_stack_pop(stack);
        pthread_mutex_unlock(&mutex);

        if (value) {
            (*value)++;
            pthread_mutex_lock(&mutex);
            my_stack_push(stack, value);
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USO: %s <nombre_fichero>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *stack_file = argv[1];
    struct my_stack *stack = my_stack_read((char *)stack_file);

    if (!stack) {
        printf("Pila no encontrada. Creando una nueva...\n");
        stack = my_stack_init(NUM_THREADS);
        if (!stack) {
            fprintf(stderr, "Error al inicializar la pila.\n");
            return EXIT_FAILURE;
        }

        for (int i = 0; i < NUM_THREADS; i++) {
            int *data = malloc(sizeof(int));
            if (!data) {
                perror("Error al asignar memoria");
                my_stack_purge(stack);
                return EXIT_FAILURE;
            }
            *data = 0;
            if (my_stack_push(stack, data) != 0) {
                fprintf(stderr, "Error al añadir elemento a la pila.\n");
                free(data);
                my_stack_purge(stack);
                return EXIT_FAILURE;
            }
        }
    }

    pthread_t threads[NUM_THREADS];
    struct thread_data t_data = {stack};

    for (size_t i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, worker, &t_data) != 0) {
            perror("Error creando el hilo");
            my_stack_purge(stack);
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error uniendo el hilo");
            my_stack_purge(stack);
            return EXIT_FAILURE;
        }
    }

    if (my_stack_write(stack, (char *)stack_file) != 0) {
        perror("Error al guardar la pila en el archivo");
        my_stack_purge(stack);
        return EXIT_FAILURE;
    }

    printf("Pila guardada correctamente en el archivo: %s\n", stack_file);
    my_stack_purge(stack);

    return EXIT_SUCCESS;
}
