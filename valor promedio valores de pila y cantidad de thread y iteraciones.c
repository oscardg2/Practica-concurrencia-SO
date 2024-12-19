    printf("Hay %d THREADS y %d ITERACIONES por cada uno \n", NUM_THREADS, MAX_COUNT);

    // Mostrar los valores actuales en la pila
    printf("Contenido de la pila (de arriba hacia abajo):\n");
    struct my_stack_node *current = stack->top;
    int sum = 0;
    int count = 0;
    while (current != NULL) {
        int *value = (int *)current->data;
        printf("%d\n", *value);
        sum += *value;  // Sumar el valor del elemento
        count++;        // Contar el número de elementos
        current = current->next;
    }

    // Calcular el promedio
    if (count > 0) {
        double average = (double)sum / count;
        printf("Promedio de los elementos de la pila: %.2f\n", average);
    } else {
        printf("La pila está vacía, no se puede calcular el promedio.\n");
    }
