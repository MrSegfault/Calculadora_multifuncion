#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "headers/abb.h"
#include "headers/TablaSimbolos.h"
#include "headers/bison.tab.h"
#include "headers/GestorErrores.h"
#include "headers/definiciones.h"
#include "headers/funciones.h"
abb A;

/*ESTE ARCHIVO SIRVE PARA DESACOPLAR EL COMPILADOR DE LA IMPLEMENTACIÓN DE LA
TABLA DE SÍMBOLOS. ESTAS FUNCIONES SE PODRÍAN LLAMAR INDEPENDIENTEMENTE DE QUE
SE TRATASE DE UN ÁRBOL, DE UNA TABLA HASH ETC. DESDE AQUÍ SE LLAMARÁN A LAS
FUNCIONES PERTINENTES DE LA VERDADERA IMPLEMENTACIÓN DE LA MISMA (EN ESTE CASO,
UN ÁRBOL).*/

typedef double varargs(int, ...);

/*Estas son los comandos, funciones y constantes, junto con sus componentes léxicos.
Se recorrerán los arrays con un bucle introduciéndolos en el árbol*/
char* comandos[]={"exit","quit","pbits","help","vars","funcs","consts","load",NULL};
int comp_lexicos[]={EOF,EOF,PBITS,HELP,VARS,FUNCS,CONSTS,LOAD};

char* constantes[]={"PI","E",NULL};
double valores[]={3.14159265359,2.71828182846};

char* funciones[]={"cos","sin","tan","acos","asin","atan","cosh","sinh","tanh","log10","log","ceil","log","exp","sqrt",NULL};
double (*punteros[])()={cos,sin,tan,acos,asin,atan,cosh,sinh,tanh,log10,log,ceil,log,exp,sqrt};

char* funciones_v[]={"avg","sum","max","min",NULL};
varargs (*punteros_v[])={average,sumatorio,maximo,minimo};

/*En la siguiente funcion se anaden las palabras reservadas al arbol creado*/
void inicializa_tabla_simbolos(){
	crea(&A); //se crea el arbol
	int i;
	tipoelem E; //se definen palabras y valores

	for(i=0;comandos[i]!=NULL;i++){ //se iteran ambas listas insertando
		E.codigo=comp_lexicos[i];
		E.lexema=(char*)calloc(strlen(comandos[i])+1,sizeof(char));
		E.apariciones=0;
		E.lineas=NULL;
		if(NULL==strcpy(E.lexema,comandos[i])){
			error(INSERCION_TABLA);
		}
		inserta(&A,E);
	}
	
	for(i=0;constantes[i]!=NULL;i++){ //se iteran ambas listas insertando
		E.codigo=CONST;
		E.lexema=(char*)calloc(strlen(constantes[i])+1,sizeof(char));
		E.apariciones=0;
		E.valor=valores[i];
		E.lineas=NULL;
		if(NULL==strcpy(E.lexema,constantes[i])){
			error(INSERCION_TABLA);
		}
		inserta(&A,E);
	}
	
	
	for(i=0;funciones[i]!=NULL;i++){ //se iteran ambas listas insertando
		E.codigo=FUNC;
		E.lexema=(char*)calloc(strlen(funciones[i])+1,sizeof(char));
		E.apariciones=0;
		E.puntero=punteros[i];
		E.lineas=NULL;
		if(NULL==strcpy(E.lexema,funciones[i])){
			error(INSERCION_TABLA);
		}
		inserta(&A,E);
	}
	
	for(i=0;funciones_v[i]!=NULL;i++){
		E.codigo=FUNC;
		E.lexema=(char*)calloc(strlen(funciones_v[i])+1,sizeof(char));
		E.apariciones=0;
		E.puntero=punteros_v[i];
		E.lineas=NULL;
		if(NULL==strcpy(E.lexema,funciones_v[i])){
			error(INSERCION_TABLA);
		}
		inserta(&A,E);
	}
	
}

/*Dada una variable, devuelve el valor consultándolo
en la tabla de símbolos*/
double consultar_valor(char *lexema){
	tipoelem buscador;
	
	buscanodo(A,lexema,&buscador);
	
	return buscador.valor;
}

/*Dado el nombre de una función, devuelve el puntero
a la misma consultándolo en la tabla de símbolos*/
fptr consultar_puntero(char *lexema){
	tipoelem buscador;
	
	buscanodo(A,lexema,&buscador);
	
	return buscador.puntero;
}


/*Dado el nombre de la variable y el valor a asignar,
asigna dicho valor a la variable en la tabla de símbolos*/
void asignar_valor(char *lexema, double val){
	tipoelem buscador;
	
	buscanodo(A,lexema,&buscador);
	buscador.valor=val;
	
	modifica(&A, lexema, buscador);
}

/*Para consultar un lexema en la tabla de símbolos
Devuelve el tipo de datos del que se trata*/
int consultar_tabla(char* lexema, int linea){
	tipoelem buscador;
	buscador.lexema=lexema;
	if(esMiembro(A,buscador)){ //si el lexema está en la tabla lo modificamos con nuevos datos
		buscanodo(A,lexema,&buscador);
		buscador.apariciones++; //se aumenta el número de apariciones y se añade la línea a su array
		buscador.lineas=(int*)realloc(buscador.lineas,buscador.apariciones*sizeof(int));
		buscador.lineas[buscador.apariciones-1]=linea;
		modifica(&A,buscador.lexema,buscador); //se modifica al nuevo nodo
		return buscador.codigo;
	}
	//si el lexema no está en la tabla, se inserta como identificador
	buscador.codigo=VAR;
	
	buscador.lexema=(char*)calloc(strlen(lexema)+1,sizeof(char));
	buscador.apariciones=1;
	buscador.lineas=(int*)malloc(sizeof(int));
	buscador.lineas[0]=linea;
	buscador.valor=0;
	
	if(NULL==strcpy(buscador.lexema,lexema)){
		error(INSERCION_TABLA);
	}
	inserta(&A,buscador);
	return buscador.codigo; //retornamos el codigo
}

/*Se destruye la tabla de símbolos, en este caso el árbol*/
void destruir_tabla_simbolos(){
	int i;
	destruye(&A); //destruimos el arbol y su contenido
}

/*Sirve para imprimir los contenidos*/
void imprime_tabla_simbolos(int tipo){ //para imprimir el arbol
	switch(tipo){
		case VAR:
			printf("\nVariables:\nNombre - Valor - Líneas\n");
		break;
		
		case FUNC:
			printf("\nFunciones:\nNombre - Puntero - Líneas\n");
		break;
		
		case CONST:
			printf("\nConstantes:\nNombre - Valor - Líneas\n");
		break;
	}
	imprime_arbol(A,tipo);
}
