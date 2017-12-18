IDIR =	./headers
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj

LIBS=-lfl -lm

_DEPS = lex.yy.h funciones.h AnalizadorSintactico.h bison.tab.h AnalizadorLexico.h definiciones.h GestorErrores.h TablaSimbolos.h abb.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o bison.tab.o funciones.o AnalizadorSintactico.o GestorErrores.o lex.yy.o TablaSimbolos.o abb.o AnalizadorLexico.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

ggg: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS) -Wall


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
