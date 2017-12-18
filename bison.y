%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "headers/lex.yy.h"
#include "headers/TablaSimbolos.h"
#include "headers/funciones.h"
#include "headers/GestorErrores.h"
void yyerror(const char* s);

/*Declaraciones de funciones implementadas
más abajo*/
void asignavariable(char *lexema,double val);
char *saca_comillas(char *s);
void imprime_ayuda();

void print_bits(double *num);
int counter=0;

/*Búfer de argumentos, aunque tiene tamaño 64
solo se utilizarán 10*/
double vals[64];

%}

/*Guarda el valor numérico o los caracteres
del componente léxico recibido*/
%union{
	double num;
	char* nomb;
}

/*Imprime una ligera descripción de los errores que se encuentra
el analizador sintáctico*/
%error-verbose

/*Declaraciión de los tokens*/
%token<num> NUM
%token<nomb> VAR
%token<nomb> CONST
%token<nomb> FUNC
%token<nomb> FICHERO
%token VARS
%token FUNCS
%token CONSTS
%token PBITS
%token EXIT
%token HELP
%token LOAD

/*Operadores disponibles, es preferible usar left para
evitar el uso de la pila*/
%left ','
%right '='
%right '?' ':'
%left LOG_OR
%left LOG_AND
%left '|'
%left '^'
%left '&'
%left EQUAL N_EQUAL
%left '<' '>' MEN_IG MAY_IG
%left LSHIFT RSHIFT
%left '-' '+'
%left '*' '/' '%'
%right '~' '!'
%left NEG
%right ELEV
%left FUNC

/*Indicamos los tipos de valores*/
%type<num> args
%type<num> exp

%%

/*Entrada de datos, también evita que se pare el programa
al detectarse un error*/
input: 
	| input line		{printf("%d > ",yylineno);}
	| error				{}
;

/*Cada línea que se introduce. Puede estar vacía, 
contener un comando, una expresión...*/
line: '\n'					{puts("");}
	| exp ';' '\n'
	| PBITS exp '\n'		{print_bits(&$2);}
	| exp '\n'				{printf ("\t%.10g\n", $1);}
	| EXIT '\n'				{YYACCEPT;}
	| VAR '=' exp '\n'		{asignavariable($1,$3);free($1);}
	| VAR '=' exp ';' '\n' 	{asignar_valor($1,$3);free($1);}
	| HELP '\n'				{imprime_ayuda();}
	| VARS '\n'				{imprime_tabla_simbolos(VAR);}
	| FUNCS '\n'			{imprime_tabla_simbolos(FUNC);}
	| CONSTS '\n'			{imprime_tabla_simbolos(CONST);}
	| LOAD FICHERO '\n'		{
							char *aux = saca_comillas($2);
							free($2);
							FILE* nueva=fopen(aux,"r");
							free(aux);
							if(nueva==NULL){
								error(LOAD_NO);
							}else{
								yyin=nueva;
							}
							}

/*Expresión matemática. Puede contener elementos como sumas, restas,
operaciones a nivel de bit, funciones... Hay recursión hasta encontrarse
con un número, una variable o una constante.*/
exp: NUM				{ $$ = $1; }
	| exp LOG_AND exp	{ $$ = $1 && $3; }
	| exp LOG_OR exp	{ $$ = $1 || $3; }
	| '!' exp			{ $$ = !$2; }
	| exp EQUAL exp		{ $$ = $1 == $3; }
	| exp N_EQUAL exp	{ $$ = $1 != $3; }
	| exp '<' exp		{ $$ = $1 < $3; }
	| exp '>' exp		{ $$ = $1 > $3; }
	| exp MEN_IG exp	{ $$ = $1 <= $3; }
	| exp MAY_IG exp	{ $$ = $1 >= $3; }
	| exp '?' exp ':' exp	{$$= $1?$3:$5; }
	| VAR				{ $$ = consultar_valor($1);free($1);}
	| CONST				{ $$ = consultar_valor($1);free($1);}
	| '~' exp			{ $$ = bit_not($2);}
	| exp '^' exp		{ $$ = bit_xor($1,$3); }
	| exp '|' exp		{ $$ = bit_or($1,$3); }
	| exp '&' exp		{ $$ = bit_and($1,$3); }
	| exp LSHIFT exp	{ $$ = bit_lshift($1,(int)$3); }
	| exp RSHIFT exp	{ $$ = bit_rshift($1,(int)$3); }
	| exp '+' exp		{ $$ = $1 + $3; }
	| exp '-' exp		{ $$ = $1 - $3; }
	| exp '*' exp		{ $$ = $1 * $3; }
	| exp '/' exp		{ $$ = $1 / $3; }
	| exp '%' exp		{ $$ = fmod($1,$3); }
	| '-' exp %prec NEG	{ $$ = -$2; }
	| '+' exp %prec NEG	{ $$ = $2; }
	| exp ELEV exp		{ $$ = pow ($1,$3); }
	| '(' exp ')'		{ $$ = $2; }
	| FUNC '(' exp args ')'	{ if(counter>10){ //Para tratar funciones. Obliga a que exista al menos un argumento
							error(ARGS_FUNC);
						  }else{ //Pedimos el puntero y llamamos a la función
						  	$$ = (*consultar_puntero($1))(counter,
						  									$3, //Indica el número de argumentos que se pasan
						  									vals[0],
						  									vals[1],
						  									vals[2],
						  									vals[3],
						  									vals[4],
						  									vals[5],
						  									vals[6],
						  									vals[7],
						  									vals[8]);
							counter=0; //Reseteamos el contador
							free($1);
						  }
						}
;

args:				{} //vacío
	| ',' exp args	{ vals[counter]=$2;counter++;} /*A cada argumento añadido, aumentamos el contador*/
;

%%

//Se llama cuando se encuentra un error
void yyerror(const char* s) {
	printf("Parse error: %s\n\n", s);
}

/*Dado el nombre de una variable y el valor, se 
llama a la tabla de símbolos para asignarlo*/
void asignavariable(char *lexema, double val){
	printf("\t%s = %.10g\n",lexema,val);
	asignar_valor(lexema,val);
}


/*Imprime la representación en bits de un valor*/
void print_bits(double *num){
	typedef unsigned char byte; //Creamos el tipo de datos byte
    byte *b = (byte*) num; //Asignamos el puntero a una variable tipo byte
    byte actual;
    int i, j;
    
    for(i=7;i>=0;i--){ //Son 8 bytes, tenemos que leer al revés
		for (j=7;j>=0;j--){//8 bits por cada byte, también al revés
			actual = (b[i] >> j) & 1; //Aplicamos el offset y hacemos un AND a nivel de bit para determinar si es 0 o 1
			printf("%u", actual); //Imprimimos el valor
		}
	}
    puts(""); //Fin de la línea
}

/*Los ficheros que se introducen con load tienen su nombre
 entre comillas, hay que sacarlas*/
char *saca_comillas(char *s){
	int i;
	int len=strlen(s); //Longitud con comillas
	char *res=calloc(len-1,sizeof(char)); //Reserva para la salida, teniendo en cuenta el \0
	for(i=0;i<len-2;i++){ //Desplazamos hacia la izquierda, se borran los "
		res[i]=s[i+1];
	}
	res[len-2]='\0'; //El último valor es \0
	return res;
}

/*Imprime ayuda sobre la calculadora*/
void imprime_ayuda(){
	printf("-----HELP-----\n");
	printf("\nBienvenido a la calculadora definitiva. En este entorno podrás realizar los cálculos que desees. Para llevar a cabo un cálculo, escriba una expresión matemática (del estilo 3+2*(5/6)**2).\n\n");
	printf("Se pueden asignar variables utilizando el operador \"=\", escribiendo líneas del estilo x=5+4.\n\n");
	printf("Si al final de una línea se escribe un \";\", no se mostrará la salida de la operación por pantalla.\n\n");
	printf("Si se desea conocer el valor de una variable, se debe escribir el nombre de la misma en una línea y pulsar enter.\n\n");
	printf("Algunos comandos útiles:\n");
	printf("\thelp -> comando de ayuda\n");
	printf("\texit -> cerrar el programa\n");
	printf("\tpbits -> Al introducir esto al comienzo de una línea se imprimirá el resultado bit a bit\n");
	printf("ATENCIÓN!. Si no se escribe pbits se obtendrá el valor en formato double, en muchos casos el sentido del número se pierde al pasarlo de binario a este formato, se aconseja utilizar pbits para visualizar el resultado.\n");
	printf("\tload -> Permite ejecutar ficheros, se utiliza de la forma: load \"fichero\"");
	printf("También se pueden utilizar funciones trigonométricas básicas. Algunos ejemplos son sin, cons y tan. Se han implementado funciones como avg, max y min. Las funciones funcionan escribiendo el nombre de la función, abriendo paréntesis, introduciendo argumentos y cerrando paréntesis (los argumentos se separan mediante el caracter \",\").\n\n");
	printf("Si se desea imprimir cierta ayuda sobre qué variables han sido utilizadas, las funciones disponibles o las constantes implementadas se puede utilizar:\n");
	printf("\tconsts\n");
	printf("\tfuncs\n");
	printf("\tvars\n");
	printf("\nExiste la posibilidad de utilizar operadores a nivel de bit (como en c), además de operadores lógicos (también siguiendo la sintaxis de c) incluyendo el condicional ternario.\n");
}

