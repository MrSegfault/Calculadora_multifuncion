%{
#include "headers/TablaSimbolos.h"
#include "headers/GestorErrores.h"
#include "headers/bison.tab.h"
#include <string.h>
%}
	/*Para contar el número de líneas*/
%option yylineno

	/*Detección de números, incluída notación científica*/
decimales [[:digit:]]+
exponente [eE][+-]?{decimales}
flotantes ({decimales}\.{decimales}?{exponente}?|{decimales}{exponente}|\.{decimales}{exponente}?)

%%

	/*Quitar espacios*/
[ \t]	;

	/*Detecta nombre entre comillas*/
\"[[:alnum:][:punct:]]+\"	{
yylval.nomb=(char*)calloc(strlen(yytext)+1,1);
strcpy(yylval.nomb,yytext);
return FICHERO;}

	/*Detecta variables, nombres de funciones...*/
[[:alpha:]_][[:alnum:]_]*	{
int tipo=consultar_tabla(yytext,yylineno);
switch(tipo){
	case VAR:
	case FUNC:
	case CONST:
		yylval.nomb=(char*)malloc(strlen(yytext)+1);
		strcpy(yylval.nomb,yytext);
}
return tipo;
}

	/*Los diferentes tipos que se pueden introducir*/
{flotantes}			{yylval.num=atof(yytext);return NUM;}
{decimales}			{yylval.num=atof(yytext);return NUM;}

	/*Operadores etc*/
\n		{return '\n';}
"+"		{return yytext[0];}
"-"		{return '-';}
"*"		{return '*';}
"/"		{return '/';}
"%"		{return '%';}
"("		{return '(';}
")"		{return ')';}
"="		{return '=';}
"**"	{return ELEV;}
";"		{return ';';}
","		{return ',';}
"&"		{return '&';}
"|"		{return '|';}
"^"		{return '^';}
"~"		{return '~';}
">>"	{return RSHIFT;}
"<<"	{return LSHIFT;}
"=="	{return EQUAL;}
"!="	{return N_EQUAL;}
"?"		{return '?';}
":"		{return ':';}
"&&"	{return LOG_AND;}
"||"	{return LOG_OR;}
"<"		{return '<';}
">"		{return '>';}
"<="	{return MEN_IG;}
">="	{return MAY_IG;}
"!"		{return '!';}

	/*Cambiar a entrada por stdin al terminar un fichero*/
<<EOF>> {
	if(yyin!=stdin && fclose(yyin)==EOF){
		error(FICHERO_CERR);
	}
	yyrestart(stdin);
	return '\n';
}

	/*Por si se introduce un caracter no contemplado*/
.		{error(CARACTER_INESPERADO);}

%%


