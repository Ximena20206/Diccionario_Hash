

//Instrucciones de compilacion
//gcc main_tablashash.c tablashash.c TADListaDL.c -o a

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablashash.h"  
#include <math.h>


int main( ){
    TablaHash tabla;
    crear_tabla(&tabla);

    int opcion;
    char palabra[100];
    char definicion[500];
    char nombre_archivo[100];
    int hashAutilizar;

    // Selección de la función de hash
    hashAutilizar = seleccionar_funcion_hash();

    do {
        mostrar_menu();
        scanf("%d", &opcion);
        getchar(); // Capturar el salto de línea después de la opción

        switch (opcion) {
            case 1:
                printf("Ingrese el nombre del archivo a cargar (debe estar en el mismo directorio): ");
                fgets(nombre_archivo, sizeof(nombre_archivo), stdin);
                nombre_archivo[strcspn(nombre_archivo, "\n")] = 0; // Eliminar el salto de línea al final
                cargar_archivo(&tabla, nombre_archivo, hashAutilizar);
                break;
            case 2:
                printf("Ingrese la palabra que desea agregar: ");
                fgets(palabra, sizeof(palabra), stdin);
                palabra[strcspn(palabra, "\n")] = 0; // Eliminar el salto de línea al final

                printf("Ingrese la definicion de '%s': ", palabra);
                fgets(definicion, sizeof(definicion), stdin);
                definicion[strcspn(definicion, "\n")] = 0; // Eliminar el salto de línea al final

                insertar(&tabla, palabra, definicion, hashAutilizar);
                break;
            case 3:
                printf("Ingrese la palabra cuya definicion desea modificar: ");
                fgets(palabra, sizeof(palabra), stdin);
                palabra[strcspn(palabra, "\n")] = 0; // Eliminar el salto de línea al final

                printf("Ingrese la nueva definicion de '%s': ", palabra);
                fgets(definicion, sizeof(definicion), stdin);
                definicion[strcspn(definicion, "\n")] = 0; // Eliminar el salto de línea al final

                modificar(&tabla, palabra, definicion, hashAutilizar);
                break;
            case 4:
                printf("Ingrese la palabra que desea eliminar: ");
                fgets(palabra, sizeof(palabra), stdin);
                palabra[strcspn(palabra, "\n")] = 0; // Eliminar el salto de línea al final

                eliminar(&tabla, palabra, hashAutilizar);
                break;
            case 5:
                printf("Ingrese la palabra que desea buscar: ");
                fgets(palabra, sizeof(palabra), stdin);
                palabra[strcspn(palabra, "\n")] = 0; // Eliminar el salto de línea al final

                char *definicion_encontrada = buscar(&tabla, palabra, hashAutilizar);
                if (definicion_encontrada != NULL) {
                    printf("Definicion de '%s': %s\n", palabra, definicion_encontrada);
                    free(definicion_encontrada); // Liberar la memoria asignada por strdup en buscar
                }
                break;
            case 6:
                consultar_estadisticas(&tabla);
                break;
            case 7:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Por favor, ingrese una opción valida.\n");
                break;
        }
    } while (opcion != 7);

    liberar_tabla(&tabla);

    return 0;
}

