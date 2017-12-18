

typedef char* tipoclave;

////////////////////////////////////////////////////////////////
typedef struct{
    char* lexema;
    int codigo;
    int* lineas;
    int apariciones;
    union{
		double valor;
		void *puntero;
    };
} tipoelem;


////////////////////////////////////////////////////////////////
		
typedef void *abb; //tipo opaco

void crea(abb *A);
void destruye(abb *A);
unsigned esVacio(abb A);
void inserta(abb *A, tipoelem E);
tipoelem suprime_min(abb *A);
void suprime(abb *A,tipoelem E);
unsigned esMiembro(abb A, tipoelem E);
abb izq(abb A);
abb der(abb A);
void info(abb A, tipoelem *E);
void buscanodo(abb A, tipoclave cl, tipoelem *nodo);
void modifica(abb *A, tipoclave cl, tipoelem nodo);
void imprime_arbol(abb A,int tipo);
