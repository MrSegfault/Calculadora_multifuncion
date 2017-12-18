typedef double (*fptr)();

void inicializa_tabla_simbolos();
int consultar_tabla(char *lexema,int linea);
void destruir_tabla_simbolos();
void imprime_tabla_simbolos();
void asignar_valor(char *lexema, double val);
double consultar_valor(char *lexema);
fptr consultar_puntero(char *lexema);
