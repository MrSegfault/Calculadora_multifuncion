#include<stdio.h>
#include<stdlib.h>
#include"headers/TablaSimbolos.h"
#include"headers/AnalizadorLexico.h"
#include"headers/definiciones.h"
#include"headers/GestorErrores.h"
#include"headers/lex.yy.h"

/*La función pedir componente léxico devuelve una estructura
cuyo contenido está formado por el lexema, el componente
léxico y el número de línea. Esta función en realidad
sirve como wrapper de los procesos llevados a cabo por flex.

En definitiva, AnalizadorLexico.c no implementa en realidad
el analizador léxico, es usado para desacoplar la implementación
de dicho analizador (en este caso flex) del resto del programa.*/
componente_lexico pedir_componente_lexico(){
	componente_lexico a;
	
	//Pedimos a flex el siguiente componente léxico
	a.codigo=yylex();
	a.lexema=yytext;
	a.linea=yylineno;
	return a;
}

/*Sirve para abrir el fichero e indicarle a lex mediante yyin que es
lo que debe leer*/
void inicializar_analizador_lexico(FILE *fichero){
	yyin=fichero;
	if(yyin==NULL){
		error(FICHERO_APERT);
	}
}

/*Libera toda la memoria, incluyendo la memoria usada para lexemas etc*/
void cerrar_analizador_lexico(){
	if(yyin!=stdin && fclose(yyin)==EOF){
		error(FICHERO_CERR); //en caso de error lo indicamos
	}
	yylex_destroy();
}
