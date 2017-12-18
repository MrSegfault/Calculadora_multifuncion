typedef struct {
	char *lexema;
	int codigo;
	int linea;
} componente_lexico;

componente_lexico pedir_componente_lexico();
void inicializar_analizador_lexico(FILE* fichero);
void cerrar_analizador_lexico();
