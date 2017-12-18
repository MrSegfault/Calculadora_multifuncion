#include<stdlib.h>
#include<stdio.h>
#include"headers/AnalizadorSintactico.h"
#include"headers/AnalizadorLexico.h"
#include"headers/lex.yy.h"
#include"headers/bison.tab.h"


void inicializar_analizador_sintactico(){
	printf("Bienvenido a la experiencia de cálculo definitiva! Si necesitas ayuda, introduce \"help\"\n1 > ");
	yyparse();
}
