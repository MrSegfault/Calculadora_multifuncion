#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/GestorErrores.h"
#include "headers/bison.tab.h"

////////////////////////////////////////////////////////////
typedef char* tipoclave;
////////////////////////////////////////////////////////////////

/*Incluye el lexema, el codigo (componente lexico),
las líneas en las que aparece y el número de apariciones
El union guarda el valor de la variable o el puntero de la función,
depende de lo que se esté almacenando*/
typedef struct{
    char* lexema;
    int codigo;
    int* lineas;
    int apariciones;
    union{
		double valor;
		double (*puntero)();
    };
} tipoelem;

/*Cada una de las celdas del árbol*/
struct celda {
    tipoelem info;
    struct celda *izq, *der;
};

/*Cada celda es en sí un árbol*/
typedef struct celda * abb;

/*Inicializa el árbol*/
void crea(abb *A) {
    *A = NULL;
}

/*Destruye el árbol celda a celda,
para lo cual avanza de forma recursiva.
También se libera el contenido de las
celdas*/
void destruye(abb *A) {
    if (*A != NULL) {
        destruye(&(*A)->izq);
        destruye(&(*A)->der);
        free((*A)->info.lexema);
        if((*A)->info.lineas!=NULL){
        	free((*A)->info.lineas);
        }
        free(*A);
        *A = NULL;
    }
}

/*Indica si el árbol pasado por argumento
todavía no contiene nada*/
unsigned esVacio(abb A) {
    return A == NULL;
}

/*Sirve para insertar un nodo en el árbol.
Para ello, compara el lexema asociado y 
sigue de forma recursiva hasta la posición
adecuada para el mismo (insertando en la
izquierda o derecha)*/
void inserta(abb *A, tipoelem E) {
    if (esVacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
    } else if (strcmp(E.lexema,(*A)->info.lexema)<0)
        inserta(&(*A)->izq, E);
    else
        inserta(&(*A)->der, E);
}


/*Busca de forma recursiva el valor mínimo
del árbol (se utiliza la estructura del mismo,
ya que el árbol se ha ordenado según el lexema).
El nodo situado más a la izquierda es el de menor valor.
Esta función no se usa en el compilador, pero ya existía
en este archivo.*/
tipoelem suprime_min(abb *A) {
    abb aux;
    tipoelem ele;
    if ((*A)->izq == NULL) {
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        free(aux);
        return ele;
    } else
        return suprime_min(&(*A)->izq);

}

/*Sirve para eliminar un elemento concreto del árbol.
Para ello se utiliza el lexema para buscar dicho elemento,
de forma recursiva. Esta función tampoco es utilizada.*/
void suprime(abb *A, tipoelem E) {
    abb aux;
    if (*A != NULL) {
        if (strcmp(E.lexema, (*A)->info.lexema)<0)
            suprime(&(*A)->izq, E);
        else if (strcmp(E.lexema, (*A)->info.lexema)>0)
            suprime(&(*A)->der, E);
        else if ((*A)->izq == NULL && (*A)->der == NULL) {
            free(*A);
            *A = NULL;
        } else if ((*A)->izq == NULL) {
            aux = *A;
            *A = (*A)->der;
            free(aux);
        } else if ((*A)->der == NULL) {
            aux = *A;
            *A = (*A)->izq;
            free(aux);
        } else
            (*A)->info = suprime_min(&(*A)->der);

    }
}


/*Se utiliza el lexema del elemento pasado para
buscar un elemento en el árbol. Se indica en el retorno
si se ha encontrado el elemento indicado. La función es recucrsiva,
llamándose a sí misma con uno de los hijos como argumento según
el lexema.*/
unsigned esMiembro(abb A, tipoelem E) {
    if (esVacio(A))
        return 0;
    else if (strcmp(E.lexema, A->info.lexema)==0)
        return 1;
    else if (strcmp(E.lexema, A->info.lexema)>0)
        return esMiembro(A->der, E);
    else
        return esMiembro(A->izq, E);
}

/*Devuelve la estructura de información del árbol indicado,
teniendo en cuenta que cada celda es de por sí un árbol.*/
void info(abb A, tipoelem *E) {
    *E = A->info;
}

/*Devuelve el hijo izquierdo de un árbol*/
abb izq(abb A) {
    return A->izq;
}

/*Devuelve el hijo derecho de un árbol*/
abb der(abb A) {
    return A->der;
}

/*Sirve para devolver el nodo buscado por la clave. Se utiliza
la clave (el lexema) para comparar y realizar la búsqueda recursiva
en los hijos. Si se encuentra, se devuelve el elemento.*/
void buscanodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (esVacio(A)){
    	error(ARBOL_VACIO);
    }
    else if (strcmp(cl, A->info.lexema)==0)
        *nodo = A->info;
    else if (strcmp(cl, A->info.lexema)<0)
        buscanodo(izq(A), cl, nodo);
    else
        buscanodo(der(A), cl, nodo);
}

/*Sirve para modificar el valor de un elemento. Para ello se realiza
la búsqueda del elemento en el árbol y se cambia la información
del árbol por el nuevo nodo pasado por argumento*/
void modifica(abb *A, tipoclave cl, tipoelem nodo) {
    if (esVacio(*A)) {
        error(ARBOL_VACIO);
    } else if (strcmp(cl, (*A)->info.lexema)==0)
        (*A)->info = nodo;
    else if (strcmp(cl, (*A)->info.lexema)<0)
        modifica(&(*A)->izq, cl, nodo);
    else
        modifica(&(*A)->der, cl, nodo);
}


/*Sirve para imprimir el árbol en inorden. Si se dispone de un hijo
izquierdo se llama a la misma función sobre el mismo, si no, se imprime
la información del nodo actual, y se llama a la función sobre el hijo derecho.
Esto resulta en el árbol impreso ordenado por lexema.*/
void imprime_arbol(abb A,int tipo){
	int i;
	if(esVacio(A)){
		return;
	}else{
		imprime_arbol(izq(A),tipo);
		if((A->info).codigo==tipo){
			printf("%s\t",(A->info).lexema);
			if(tipo==FUNC){
				printf("%p\t",(A->info).puntero);
			}else{
				printf("%2f\t",(A->info).valor);
			}
			for(i=0;i<(A->info).apariciones;i++){
				printf("%d ",(A->info).lineas[i]);
			}
			puts("");
		}
		imprime_arbol(der(A),tipo);
	}
}
