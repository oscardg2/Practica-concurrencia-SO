//Realizado por Antonio Jose Perez Garcia, Oscar Delgado Galera y Jiasheng Liu

#include "my_lib.h"

size_t my_strlen(const char *str) {
    size_t longitud = 0;  // Contador para la longitud

    // Recorremos la cadena hasta encontrar el carácter nulo '\0'
    while (str[longitud] != '\0') {
        longitud++;
    }

    return longitud;  // Retornamos la longitud calculada
}

int my_strcmp(const char *str1, const char *str2)
{
    char c1 = *str1;
    char c2 = *str2;
    int diferencia = 0;
    int aux = 1;

    while (1)
    {
        if ((c1 == '\0') & (c2 != '\0'))
        {
            return -1;
        }
        else if ((c1 != '\0') & (c2 == '\0'))
        {
            return 1;
        }
        if (c1 != c2)
        {
            diferencia = c1 - c2;
            return diferencia;
        }
        else
        {
            c1 = *(str1 + aux);
            c2 = *(str2 + aux);
            aux++;
        }
        if ((c1 == '\0') & (c2 == '\0'))
        {
            return 0;
        }
    }
}

char *my_strcpy(char *dest, const char *src)
{
    for (size_t i = 0; i <= strlen(src); i++)
        dest[i] = src[i];

    return dest;
}
char *my_strncpy(char *dest, const char *src, size_t n) {
    size_t i = 0;

    // Copia hasta n caracteres de src a dest, o hasta el final de src
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }

    // Si n > strlen(src), rellena el resto de dest con '\0'
    for (; i < n; i++) {
        dest[i] = '\0';
    }

    return dest;
}
char *my_strcat(char *dest, const char *src)
{
    const char *p;
    char *q;

    for (q = dest; *q != '\0'; q++)
        ;
    for (p = src; *p != '\0'; p++, q++)
        *q = *p;
    *q = '\0';
    return dest;
}

char *my_strchr(const char *str, int c)
{
    while (*str != c && *str != '\0')
    {
        str++;
    }
    if (*str == c)
    {
        return (char *)str;
    }
    else
    {
        return NULL;
    }
}

// Inicializa la pila con el tamaño de datos especificado
struct my_stack *my_stack_init(int size) {
    struct my_stack *ptr = malloc(sizeof(struct my_stack));
    if (ptr == NULL) {
        return NULL; // Devuelve NULL si falla la asignación de memoria
    }
    ptr->size = size;
    ptr->top = NULL; // Inicialice top en NULL para una pila vacía
    return ptr;
}

int my_stack_push(struct my_stack *stack, void *data) {
    if (stack == NULL || stack->size == 0 || data == NULL) {
        return -1; // Error: pila o tamaño de datos no válido
    }

    struct my_stack_node *new_node = (struct my_stack_node *)malloc(sizeof(struct my_stack_node));
    if (new_node == NULL) {
        return -1; // Error: la asignación de memoria falló
    }

    new_node->data = malloc(stack->size);
    if (new_node->data == NULL) {
        free(new_node);
        return -1; // Error: la asignación de memoria falló
    }

    memcpy(new_node->data, data, stack->size);
    new_node->next = stack->top;
    stack->top = new_node;

    return 0; // Éxito
}


int my_stack_purge(struct my_stack *stack) {
    if (stack == NULL) {
        return 0;
    }

    int bytes_freed = sizeof(struct my_stack);  // Tamaño de la estructura de la pila
    struct my_stack_node *current = stack->top;
    while (current != NULL) {
        struct my_stack_node *next = current->next;

        // Sumar el tamaño del nodo y de su data según stack->data_size
        bytes_freed += sizeof(struct my_stack_node);
        if (current->data != NULL) {
            bytes_freed +=stack->size;
            free(current->data);
        }
        free(current);
        current = next;
    }

    free(stack);  // Liberar la estructura de la pila
    return bytes_freed;
}
void *my_stack_pop(struct my_stack *stack) {
    //Verifica si la pila esta vacia
    if (stack->top == NULL) {
        return NULL;  //No hay nodos en la pila
    }

    // Guardar el nodo superior de la pila
    struct my_stack_node *node_to_pop = stack->top;

    // Actualizar el puntero top al siguiente nodo en la pila
    stack->top = node_to_pop->next;

    // Obtener el puntero a los datos almacenados en el nodo
    void *data = node_to_pop->data;

    // Liberar la memoria del nodo extraído (pero no los datos)
    free(node_to_pop);

    // Devolver el puntero a los datos
    return data;
}

// Get stack size by counting nodes
int my_stack_len(struct my_stack *stack) {
    int count = 0;
    struct my_stack_node *current = stack->top;

    while (current != NULL) {
        count++;
        current = current->next;
    }

    return count;
}

struct my_stack *my_stack_read(char *filename)
{
    int fd = open(filename, O_RDONLY); // modo solo lectura
    if (fd == -1)
    { // tratamiento de error
        perror("Error abriendo el archivo");
        return NULL;
    }

    int size; // Tamaño de los datos
    // Recuerda que en Write dejas al principio de fichero sizeof(int) byte de espacio para guardar el numero 'tamaño'
    if (read(fd, &size, sizeof(int)) != sizeof(int))
    { // Leer este numero y lo dejas en variable size.
        perror("Error leyendo el tamaño de los datos");
        close(fd);
        return NULL; // Error al leer el tamaño
    }

    // Inicializar la pila con el tamaño leído
    struct my_stack *stack = my_stack_init(size);
    if (stack == NULL)
    {
        perror("Error inicializando la pila");
        close(fd);
        return NULL;
    }

    void *data = malloc(size); // Reserva de memoria para los datos de cada nodo
    if (data == NULL)
    {
        perror("Error reservando memoria");
        close(fd);
        return NULL;
    }

    // Leer datos y lo dejas en data
    while (read(fd, data, size) > 0)
    {
        if (data == NULL)
        {
            perror("Error reservando memoria para los datos");
            free(data); // Liberar la memoria inicial antes de retornar
            close(fd);
            return NULL;
        }

        // Push dato leido que está en data
        if (my_stack_push(stack, data) != 0)
        {
            perror("Error haciendo push a la pila");
            free(data);
            // Liberar memoria en caso de error
            close(fd);
            return NULL;
        }
        data = malloc(size);
        if (data == NULL)
        {
            perror("Error reservando memoria");
            close(fd);
            return NULL;
        }
    }

    // Limpiar y cerrar
    free(data); // Liberar la memoria temporal usada para leer
    close(fd);  // Cerrar el archivo

    return stack; // Retornar la pila reconstruida
}

// Implementación de my_stack_write
int my_stack_write(struct my_stack *stack, char *filename) {
    // Abrir el archivo para escritura
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return -1;  // Error al abrir el archivo
    }

    // Escribir el tamaño de los datos en el archivo
    if (write(fd, &(stack->size), sizeof(int)) != sizeof(int)) {
        close(fd);
        return -1;  // Error al escribir el tamaño
    }

    // Crear una pila auxiliar para preservar el orden
    struct my_stack *aux_stack = my_stack_init(stack->size);
    if (!aux_stack) {
        close(fd);
        return -1;  // Error al crear la pila auxiliar
    }

    // Transferir los datos de la pila original a la auxiliar
    struct my_stack_node *current = stack->top;
    while (current != NULL) {
        // Crear espacio para el dato actual
        void *data_copy = malloc(stack->size);
        if (!data_copy) {
            my_stack_purge(aux_stack);
            close(fd);
            return -1;  // Error al asignar memoria
        }

        // Copiar cada byte manualmente del dato actual
        char *src = (char *)current->data;
        char *dst = (char *)data_copy;
        for (int i = 0; i < stack->size; i++) {
            dst[i] = src[i];
        }

        // Apilar el dato copiado en la pila auxiliar
        if (my_stack_push(aux_stack, data_copy) != 0) {
            free(data_copy);
            my_stack_purge(aux_stack);
            close(fd);
            return -1;  // Error al apilar en la auxiliar
        }

        // Avanzar al siguiente nodo
        current = current->next;
    }

    // Escribir los datos de la pila auxiliar en el archivo
    int count = 0;
    void *data;
    while ((data = my_stack_pop(aux_stack)) != NULL) {
        // Escribir el dato en el archivo
        if (write(fd, data, stack->size) != stack->size) {
            free(data);
            my_stack_purge(aux_stack);
            close(fd);
            return -1;  // Error al escribir el dato
        }
        free(data);
        count++;
    }

    // Liberar la pila auxiliar y cerrar el archivo
    my_stack_purge(aux_stack);
    close(fd);

    return count;  // Retornar el número de elementos almacenados
}
