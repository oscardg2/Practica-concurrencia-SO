#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_lib.h" // Declaraciones del gestor de pila
#define _POSIX_C_SOURCE 200112L
#define NUM_THREADS 10

int main(int argc, char *argv[]) {
    // Verificar argumentos
    if (argc != 2) {
        fprintf(stderr, "USAGE: %s <stack_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Variables
    const char *stack_file = argv[1];
    struct my_stack *stack = NULL;

    // Intentar leer la pila desde el archivo
    stack = my_stack_read((char *)stack_file);
    if (stack == NULL) {
        printf("Pila no encontrada. Creando una nueva...\n");
        stack = my_stack_init(NUM_THREADS);
        if (stack == NULL) {
            fprintf(stderr, "Error al inicializar la pila.\n");
            return EXIT_FAILURE;
        }

        // Inicializar la pila con 10 punteros apuntando a 0
        for (int i = 0; i < NUM_THREADS; i++) {
            int *data = malloc(sizeof(int));
            if (!data) {
                perror("Error al asignar memoria");
                my_stack_purge(stack);
                return EXIT_FAILURE;
            }
            *data = 0; // Inicializar a 0
            if (my_stack_push(stack, data) != 0) {
                fprintf(stderr, "Error al añadir elemento a la pila.\n");
                free(data);
                my_stack_purge(stack);
                return EXIT_FAILURE;
            }
        }
        printf("Pila creada e inicializada con 10 elementos apuntando a 0.\n");
    } else {
        // Verificar el tamaño de la pila
        int stack_size = my_stack_len(stack);
        printf("Pila encontrada con %d elementos.\n", stack_size);

        if (stack_size < NUM_THREADS) {
            printf("Añadiendo elementos para alcanzar los 10 requeridos...\n");
            for (int i = stack_size; i < NUM_THREADS; i++) {
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
    }

    // Mostrar el contenido inicial de la pila
    printf("Contenido inicial de la pila:\n");
    int stack_size = my_stack_len(stack);
    for (int i = 0; i < stack_size; i++) {
        void *data = my_stack_pop(stack); // Extraer temporalmente cada elemento
        if (data != NULL) {
            printf("Elemento %d: %d\n", i, *(int *)data);
            my_stack_push(stack, data); // Reinsertar en la pila
        }
    }

    // Guardar la pila en el archivo
    if (my_stack_write(stack, (char *)stack_file) != 0) {
        perror("Error al guardar la pila en el archivo");
        my_stack_purge(stack);
        return EXIT_FAILURE;
    }

    printf("Pila guardada correctamente en el archivo: %s\n", stack_file);

    // Liberar memoria de la pila
    if (my_stack_purge(stack) != 0) {
        fprintf(stderr, "Error al liberar la memoria de la pila.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
