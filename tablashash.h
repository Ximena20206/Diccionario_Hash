/*
LIBRERIA: Cabecera de Diccionario con Tabla Hash Abierta

AUTORES:	Rocha Arellano Ximena Yulian
			        Bautista Romero Demian
(C)	Junio 2024
VERSIÓN: 1.0

DESCRIPCIÓN: tablashash.h
Se declara un arreglo de listas para poder crear una tabla
hash abierta en la cual se meteran palabras y definciones
las cuales serán introducidas por 2 funciones de hasheo distintas.

OBSERVACIONES: 
La lista es simplemente ligada 

*/

#ifndef TABLA_HASH_H
#define TABLA_HASH_H
#include "TADListaDL.h"
//Constante global con la que controlamos el tamaño de la tabla hash
#define TAMANO_TABLA_HASH 1499

// Estructura para la tabla hash
typedef struct {
    lista arreglo[TAMANO_TABLA_HASH]; // Arreglo de listas enlazadas
    int tamano;           // Tamaño del arreglo (número de listas)
} TablaHash;

// Funciones para manejar la tabla hash

// Crea una tabla hash con un número especificado de listas
void crear_tabla(TablaHash *tabla);

// Función de hash 1.1 utilizando la razón áurea
int funcion_hash1(int clave);


// Función de hash 2.1 utilizando el número euler
int funcion_hash2(int clave);


// Función para convertir texto a entero usando valores ASCII
int textoAint(const char *clave);

// Inserta un par palabra-definición en la tabla hash
void insertar(TablaHash *tabla, const char *palabra, const char *definicion, int hashAutilizar);

// Inserta un par palabra-definición en la tabla hash
void insertarArchivo(TablaHash *tabla, const char *palabra, const char *definicion, int hashAutilizar);

// Busca una definición en la tabla hash dada una palabra
char* buscar(TablaHash *tabla, const char *palabra, int hashAutilizar);

// Elimina un par palabra-definición de la tabla hash dada una palabra
void eliminar(TablaHash *tabla, const char *palabra, int hashAutilizar);

// Libera toda la memoria utilizada por la tabla hash
void liberar_tabla(TablaHash *tabla);

// Carga un archivo de definiciones en la tabla hash
void cargar_archivo(TablaHash *tabla, const char *nombre_archivo, int hashAutilizar);

// Consulta un resumen estadístico de la distribución de palabras, colisiones y eficiencia de la tabla hash
void consultar_estadisticas(TablaHash *tabla);

//Modificamos una palabra o definicion o ambos
void modificar(TablaHash *tabla, const char *palabra, const char *nueva_definicion, int hashAutilizar);

//Función para mostrar el menú en el main
void mostrar_menu();

//Función para elegir la función hash
int seleccionar_funcion_hash();


#endif // TABLA_HASH_H


