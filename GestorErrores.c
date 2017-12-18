#include<stdio.h>
#include<stdlib.h>
#include"headers/GestorErrores.h"
#include"headers/TablaSimbolos.h"

/*A continuación se definen las acciones a llevar a cabo según el error recibido.
En algunos de los casos es necesario liberar memoria. Sólo se cierra el compilador
cuando es imposible seguir analizando (algún error de las funciones de C utilizadas,
como calloc o apertura de ficheros).*/
void error(errores errnum){
	switch(errnum){
		case ARGUMENTOS:
			printf("NUMERO INCORRECTO DE ARGUMENTOS: Se debe indicar solo el fichero a compilar\n");
			exit(-1);
		case FICHERO_APERT:
			perror("ERROR ABRIENDO FICHERO: ");
			exit(-1);
		break;
		
		case FICHERO_CERR:
			perror("ERROR CERRANDO FICHERO: ");
			return; //retornamos, ya que el siguiente paso sería ya liberar tabla.
			
		case CARACTER_INESPERADO:
			printf("\n\nSE HA ENCONTRADO UN CARACTER INESPERADO\n\n\n");
			return;
		break;
		
		case FIN_COMENTARIO:
			printf("SE HA LLEGADO AL FINAL DEL FICHERO DENTRO DE UN COMENTARIO\n");
		break;
		
		case LECTURA:
			perror("HA OCURRIDO UN ERROR DURANTE LA LECTURA DEL FICHERO: ");
		break;
		
		case LEXEMA_GRANDE:
			printf("UNO DE LOS LEXEMAS DEL CODIGO A COMPILAR ES DEMASIADO GRANDE\n");
			return;
		break;
		
		case CREACION_LEXEMA:
			perror("HA OCURRIDO UN ERROR DURANTE LA GENERACION DE UN LEXEMA: ");
		break;
		
		case FIN_STRING:
			printf("SE HA LLEGADO AL FINAL DEL FICHERO MIENTRAS SE LEIA UN STRING\n");
		break;
		
		case ARBOL_VACIO:
			printf("EL ARBOL NO EXISTE\n");
		break;
		
		case ARGS_FUNC:
			printf("LA FUNCIÓN ADMITE UN MÁXIMO DE 10 ARGUMENTOS\n");
			return;
		break;
		
		case INSERCION_TABLA:
			perror("SE HA DADO UN FALLO AL INTENTAR INTRODUCIR UN VALOR EN LA TABLA DE SIMBOLOS: ");
		break;
		
		case LOAD_NO:
			perror("ERROR AL ABRIR EL FICHERO INDICADO: ");
			return;
		break;
		
		default:
			//Por si se ha detectado un error no definido anteriormente
			perror("HA OCURRIDO UN ERROR: ");
		break;
	}
	
	destruir_tabla_simbolos();
	exit(-1);
}
