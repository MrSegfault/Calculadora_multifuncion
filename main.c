#include<stdio.h>
#include<stdlib.h>
#include"headers/AnalizadorSintactico.h"
#include"headers/TablaSimbolos.h"
#include"headers/GestorErrores.h"
#include"headers/AnalizadorLexico.h"

/*Se inicializan todos los elementos.
Una vez se concluye la ejecución, se destruyen
los mismos.*/
int main(int argc, char** argv){
	//debemos mirar que el número de argumentos sea adecuado
	if(argc== 2){
		//inicializamos el analizador lexico sobre el archivo
		inicializar_analizador_lexico(fopen(argv[1],"r"));	
	}else if(argc == 1){
		//inicializamos el analizador lexico en entrada
		inicializar_analizador_lexico(stdin);
	}else{
		error(ARGUMENTOS);
	}
	
	
	
	//inicializamos la tabla de símbolos
	inicializa_tabla_simbolos();

	//inicializamos el analizador sintáctico
	inicializar_analizador_sintactico();
	
	//cerramos el analizador lexico
	cerrar_analizador_lexico();
	
	//destruimos la tabla de símbolos
	destruir_tabla_simbolos();
}
