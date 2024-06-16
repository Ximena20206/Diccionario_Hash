#include "tablashash.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constantes para las funciones de hash
const double PHI = 1.618033988749895;    // Razón áurea
const double SQRT2 = 1.414213562373095;  // Raíz cuadrada de 2
const double E = 2.718281828459045;      // Número euler


void crear_tabla(TablaHash *tabla) {
    tabla->tamano = TAMANO_TABLA_HASH;
    for (int i = 0; i < tabla->tamano; i++) {
        Initialize(&(tabla->arreglo[i]));
    }
}

/*  Entramos a la referencia de la clave la cual en memoria tiene los valores en codigo ascii,
    vamos multiplicando el valor por 31 para obtener una distribución más "uniforme" y vamos acumulandolo
     retornamos el valor devolviendo a int*/

int textoAint(const char *clave) {
    unsigned long hash = 0;
    while (*clave) {
        hash = hash * 31 + *clave++;
    }
    return (int)hash;
}

//Lo realizamos por el metodo de la división haciendo uso de la razón aurea como constante
int funcion_hash1(int clave) {
    double A = PHI; // Razón áurea 
    double valor = clave * A;
    double parte_fraccionaria = valor - floor(valor);
    int hash1 = (int)(TAMANO_TABLA_HASH * parte_fraccionaria);
    return hash1 % TAMANO_TABLA_HASH;
}


//Lo realizamos utilizando de apoyo el metodo de la division agregando funciones trigonometricas (seno) como extra y el número de Euler como constante
int funcion_hash2(int clave) {
    double A = E;
    double valor = clave * A;
    double parte_fraccionaria = valor - floor(valor);
    int hash1 = (int)(TAMANO_TABLA_HASH * parte_fraccionaria);
    return (hash1 + (int)(sin(parte_fraccionaria) * 10000)) % TAMANO_TABLA_HASH;
}


//Función para insertar palabras por individual
void insertar(TablaHash *tabla, const char *palabra, const char *definicion, int hashAutilizar) {
    int clave = textoAint(palabra);
    int hash;

    // Seleccionar la función de hash a utilizar
    if (hashAutilizar == 1) {
        hash = funcion_hash1(clave);
    } else if (hashAutilizar == 2) {
        hash = funcion_hash2(clave);
    } else {
        printf("\nError: Función de hash no válida.\n");
        return;
    }

    int indice = hash % tabla->tamano;
    int colisiones = 0;

    // Verificar si la palabra ya existe en la lista
    posicion pos = First(&tabla->arreglo[indice]);
    while (ValidatePosition(&tabla->arreglo[indice], pos)) {
        elemento e = Position(&tabla->arreglo[indice], pos);
        if (strcmp(e.palabra, palabra) == 0) {
            // Ya existe la misma palabra en la tabla
            if (strcmp(e.definicion, definicion) == 0) {
                printf("La definición para la palabra '%s' ya existe en la tabla.\n", palabra);
            } else {
                printf("La palabra '%s' ya existe en la tabla con una definición diferente.\n", palabra);
            }
            return;
        }
        colisiones++;
        pos = Following(&tabla->arreglo[indice], pos);
    }

    // Insertar en la lista en el índice correspondiente
    elemento e;
    strncpy(e.palabra, palabra, sizeof(e.palabra) - 1);
    e.palabra[sizeof(e.palabra) - 1] = '\0'; // Asegurarse de que la palabra está terminada en NULL
    strncpy(e.definicion, definicion, sizeof(e.definicion) - 1);
    e.definicion[sizeof(e.definicion) - 1] = '\0'; // Asegurarse de que la definición está terminada en NULL
    Add(&tabla->arreglo[indice], e);

    // Mostrar estadísticas
    printf("Insertar: Palabra '%s', Hash: %d, Colisiones: %d\n", palabra, hash, colisiones);
}



//Función para insertar exclusivo para los archivos ya que al tener algunos printf dentro de la funcion "insertar" 
//y al insertar varias palabras se imprimian excesivos msjs
void insertarArchivo(TablaHash *tabla, const char *palabra, const char *definicion, int hashAutilizar) {
    int clave = textoAint(palabra);
    int hash = 0;

    // Seleccionar la función de hash a utilizar
    if (hashAutilizar == 1) {
        hash = funcion_hash1(clave);
    } else if (hashAutilizar == 2) {
        hash = funcion_hash2(clave);
    } else {
        printf("\nError: Función de hash no válida.\n");
        return;
    }

    int indice = hash % tabla->tamano;
    int colisiones = 0;

    // Insertar en la tabla hash sin resolver colisiones
    elemento e;
    strncpy(e.palabra, palabra, sizeof(e.palabra) - 1);
    e.palabra[sizeof(e.palabra) - 1] = '\0'; // Asegurarse de que la palabra está terminada en NULL
    strncpy(e.definicion, definicion, sizeof(e.definicion) - 1);
    e.definicion[sizeof(e.definicion) - 1] = '\0'; // Asegurarse de que la definición está terminada en NULL

    // Insertar el elemento en el índice calculado
    Add(&tabla->arreglo[indice], e);

    // Contar las colisiones sin resolverlas
    while (!Empty(&tabla->arreglo[indice])) {
        colisiones++;
        indice = (indice + 1) % tabla->tamano; // Avanzar al siguiente índice sin resolver colisiones
    }


    // Incrementar el contador de colisiones
    colisiones++;
}




/*Función para buscar palabras en especifico 
el algoritmo que tenemos actualmente esta dado por calcular la función de hashing y recorrer los elementos de las listas
hasta encontrar el que buscamos, si no llegara a estar se recorre el demás arreglo con prueba lineal hasta hallarlo, si
llegasemos a estar en el final del arreglo y no lo encontramos retornamos  y decimos que el elemento no esta"*/

char* buscar(TablaHash *tabla, const char *palabra, int hashAutilizar) {
    int clave = textoAint(palabra);
    int hash;

    // Seleccionar la función de hash a utilizar
    if (hashAutilizar == 1) {
        hash = funcion_hash1(clave);
    } else if (hashAutilizar == 2) {
        hash = funcion_hash2(clave);
    } else {
        printf("\nError: Función de hash no válida.\n");
        return NULL;
    }

    int indice = hash % tabla->tamano;
    int colisiones = 0;
    int saltos = 0; // Contador de saltos realizados (En que lugar se encuentra de la lista)

    while (!Empty(&tabla->arreglo[indice])) {
        elemento e;
        posicion pos = First(&tabla->arreglo[indice]);
        while (ValidatePosition(&tabla->arreglo[indice], pos)) {
            e = Position(&tabla->arreglo[indice], pos);
            if (strcmp(e.palabra, palabra) == 0) {
                // Mostrar estadísticas
                    printf("\nBuscar: Palabra '%s' encontrada, Hash: %d, Colisiones: %d, Saltos: %d\n", palabra, indice, colisiones, saltos);
                // Devolver una copia de definicion
                char *definicion_copia = strdup(e.definicion); //strdup copia el contenido de la cadena original en la nueva área de memoria asignada
                return definicion_copia;
            }
            saltos++;
            pos = Following(&tabla->arreglo[indice], pos);
        }
        colisiones++;
        saltos++;
        indice = (indice + 1) % tabla->tamano; // Linear probing para encontrar la siguiente posición
    }

    // Mostrar estadísticas (no encontrada)
    printf("Buscar: Palabra '%s' no encontrada, Hash: %d, Colisiones: %d, Saltos: %d\n", palabra, indice, colisiones, saltos);
    return NULL;
}


//Función para eliminar una palabra y por ende su definición
void eliminar(TablaHash *tabla, const char *palabra, int hashAutilizar) {
    int clave = textoAint(palabra);
    int hash;

    // Seleccionar la función de hash a utilizar
    if (hashAutilizar == 1) {
        hash = funcion_hash1(clave);
    } else if (hashAutilizar == 2) {
        hash = funcion_hash2(clave);
    } else {
        printf("\nError: Función de hash no válida.\n");
        return;
    }

    int indice = hash % tabla->tamano;
    int colisiones = 0;
    int saltos = 0; // Contador de saltos realizados

    while (!Empty(&tabla->arreglo[indice])) {
        posicion pos = First(&tabla->arreglo[indice]);
        while (ValidatePosition(&tabla->arreglo[indice], pos)) {
            elemento e = Position(&tabla->arreglo[indice], pos);
            if (strcmp(e.palabra, palabra) == 0) {
                Remove(&tabla->arreglo[indice], pos);
                // Mostrar estadísticas
                    printf("\nEliminar: Palabra '%s' eliminada, Hash: %d, Colisiones: %d, Saltos: %d\n", palabra, indice, colisiones, saltos);
                return;
            }
            saltos++;
            pos = Following(&tabla->arreglo[indice], pos);
        }
        colisiones++;
        saltos++;
        indice = (indice + 1) % tabla->tamano; // Linear probing para manejar colisiones
    }

    // Mostrar estadísticas (no encontrada)
    printf("\nEliminar: Palabra '%s' no encontrada, Hash: %d, Colisiones: %d, Saltos: %d\n", palabra, indice, colisiones, saltos);
}



void modificar(TablaHash *tabla, const char *palabra, const char *nueva_definicion, int hashAutilizar) {
    int clave = textoAint(palabra);
    int hash;

    // Seleccionar la función de hash a utilizar
    if (hashAutilizar == 1) {
        hash = funcion_hash1(clave);
    } else if (hashAutilizar == 2) {
        hash = funcion_hash2(clave);
    } else {
        printf("Error: Función de hash no válida.\n");
        return;
    }

    int indice = hash % tabla->tamano;
    int colisiones = 0;
    int saltos = 0; // Contador de saltos realizados

    while (!Empty(&tabla->arreglo[indice])) {
        elemento e;
        posicion pos = First(&tabla->arreglo[indice]);
        while (ValidatePosition(&tabla->arreglo[indice], pos)) {
            e = Position(&tabla->arreglo[indice], pos);
            if (strcmp(e.palabra, palabra) == 0) {
                // Encontrado: modificar la definición
                strncpy(e.definicion, nueva_definicion, 500);
                Replace(&tabla->arreglo[indice], pos, e); // Actualizar el elemento modificado

                // Mostrar estadísticas de la modificación
                    printf("Modificar: Palabra '%s' modificada, Hash: %d, Colisiones: %d, Saltos: %d\n", palabra, indice, colisiones, saltos);
                return;
            }
            saltos++;
            pos = Following(&tabla->arreglo[indice], pos);
        }
        colisiones++;
        saltos++;
        indice = (indice + 1) % tabla->tamano; // Linear probing para manejar colisiones
    }

    // Mostrar estadísticas (no encontrada)
    printf("Modificar: Palabra '%s' no encontrada, Hash: %d, Colisiones: %d, Saltos: %d\n", palabra, indice, colisiones, saltos);
}


//Ciclo para liberar tabla por tabla
void liberar_tabla(TablaHash *tabla) {
    for (int i = 0; i < tabla->tamano; ++i) {
        Destroy(&tabla->arreglo[i]);
    }
    free(tabla);
}


// Carga un archivo de definiciones en la tabla hash
void cargar_archivo(TablaHash *tabla, const char *nombre_archivo, int hashAutilizar) {
    FILE *archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        fprintf(stderr, "Error al abrir el archivo.\n");
        return;
    }

    char linea[512]; // Asumimos que ninguna línea del archivo superará los 512 caracteres

    while (fgets(linea, sizeof(linea), archivo)) {
        // Buscar el primer ':' que separa las palabras de la definición
        char *token_palabra = strtok(linea, ":");
        if (!token_palabra) continue; // Si no hay ':' en la línea, saltar a la siguiente

        // Con isspace quitamos los espacios si llegaran a haber del inicio y el final del token para quitar espacios al inicio y final
        while (isspace((unsigned char)*token_palabra)) token_palabra++;
        size_t len = strlen(token_palabra);
        while (len > 0 && isspace((unsigned char)token_palabra[len - 1])) len--;
        token_palabra[len] = '\0'; // Terminación de la cadena

        // Ahora token_palabra contiene la palabra o palabras antes del ':'
        // Buscar definición después del ':'
        char *definicion = strtok(NULL, "."); //Así lo detenenmos
        if (definicion) {
            // Trim de la definición para quitar espacios al inicio y final
            while (isspace((unsigned char)*definicion)) definicion++;
            len = strlen(definicion);
            while (len > 0 && isspace((unsigned char)definicion[len - 1])) len--;
            definicion[len] = '\0'; // Terminación de la cadena

            // Insertar en la tabla hash
            insertarArchivo(tabla, token_palabra, definicion, hashAutilizar);
        }
    }

    fclose(archivo);
}

void consultar_estadisticas(TablaHash *tabla) {
    int num_palabras = 0;
    int num_colisiones = 0;
    int num_listas_vacias = 0;
    int tamano_max_lista = 0;
    int orden_max_busqueda = 0;

    // Recorre cada índice de la tabla hash
    for (int i = 0; i < tabla->tamano; ++i) {
        // Obtener el tamaño de la lista actual
        int tamano_lista = Size(&tabla->arreglo[i]);

        // Contabilizar el número total de palabras
        num_palabras += tamano_lista;

        // Actualizar el tamaño máximo de lista
        if (tamano_lista > tamano_max_lista) {
            tamano_max_lista = tamano_lista;
        }

        // Contabilizar listas vacías
        if (tamano_lista == 0) {
            num_listas_vacias++;
        }

        // Calcular colisiones
        if (tamano_lista > 1) {
            num_colisiones += (tamano_lista - 1);
        }

        // Calcular el orden máximo de búsqueda en cada lista no vacía
        int pasos_max = 0;
        if (tamano_lista > 0) {
            posicion pos = First(&tabla->arreglo[i]);

            // Iterar a través de la lista
            while (ValidatePosition(&tabla->arreglo[i], pos)) {
                pasos_max++;
                pos = Following(&tabla->arreglo[i], pos);
            }
        }
        
        // Actualizar el orden máximo de búsqueda
        if (pasos_max > orden_max_busqueda) {
            orden_max_busqueda = pasos_max;
        }
;
    }

    // Imprimir estadísticas finales
    printf("\n***************************************************\n");
    printf("Estadisticas:\n");
    printf("Numero total de palabras: %d\n", num_palabras);
    printf("Numero total de colisiones: %d\n", num_colisiones);
    printf("Numero de listas vacias: %d\n", num_listas_vacias);
    printf("Tamano maximo de una lista: %d\n", tamano_max_lista);
    printf("Orden maximo de busqueda: %d\n", orden_max_busqueda);
    printf("Tamano de la tabla hash: %d\n", tabla->tamano);
    printf("\n***************************************************\n");
}

//Función que muestra nuestro menú de opciones 
void mostrar_menu(){
    printf("\nMenu:\n");
    printf("1. Cargar un archivo de definiciones\n");
    printf("2. Agregar una palabra y su definicion\n");
    printf("3. Modificar la definicion de una palabra\n");
    printf("4. Eliminar una palabra\n");
    printf("5. Buscar una palabra\n");
    printf("6. Ver estadisticas de la tabla hash\n");
    printf("7. Salir\n");
    printf("Ingrese el numero de la opcion que desea ejecutar: ");
}

//Con eata función elegimos entre las 2 alternativas de hash que tenemos
int seleccionar_funcion_hash(){
    int opcion;
    do {
        printf("\nSeleccione la funcion de hash que desea utilizar:\n");
        printf("1. Funcion de hash 1 (Hecha con la razón aurea):\n");
        printf("2. Funcion de hash 2 (Hecha con el numero Euler y la funcion trigonometrica Seno): \n");
        printf("Ingrese el numero de la opcion que desea utilizar: ");
        scanf("%d", &opcion);
        getchar(); // Capturar el salto de línea después de la opción
        if (opcion != 1 && opcion != 2) {
            printf("Opcion no valida. Por favor, ingrese 1 o 2.\n");
        }
    } while (opcion != 1 && opcion != 2);
    return opcion;
}





