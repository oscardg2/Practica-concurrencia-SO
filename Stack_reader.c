#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <limits.h> // Para INT_MAX
#include <string.h>
#include "my_lib.h" // Declaraciones del gestor de pila

// Función para leer y mostrar estadísticas de la pila
void print_stack_stats(const char *filename) {
    int *stack;
    int size = 0;

    // Leer la pila desde el archivo
    stack = my_stack_read(filename, &size);
    if (!stack) {
        fprintf(stderr, "Error al leer la pila desde el archivo: %s\n", filename);
        return;
    }

    int suma = 0, min = INT_MAX, max = INT_MIN;
    double media = 0.0;

    printf("\nElementos de la pila:\n");
    for (int i = 0; i < size; i++) {
        printf("Elemento %d: %d\n", i, stack[i]);
        suma += stack[i];
        if (stack[i] < min) min = stack[i];
        if (stack[i] > max) max = stack[i];
    }

    media = (double)suma / size;
    printf("\nEstadísticas:\n");
    printf("Suma: %d\n", suma);
    printf("Mínimo: %d\n", min);
    printf("Máximo: %d\n", max);
    printf("Media: %.2f\n", media);

    free(stack);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "USO: %s <nombre_fichero>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    print_stack_stats(filename);

    return EXIT_SUCCESS;
}
