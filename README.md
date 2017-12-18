# Calculadora_multifuncion
##GUÍA RÁPIDA DE COMPILACIÓN:
flex --header-file=headers/lex.yy.h lex.l
bison bison.y  --defines=headers/bison.tab.h
make

##CÓMO COMPILAR

- Consejos:
	- Utilizar una distribución de linux
	- Utilizar bash
	- Disponer de GNU Make 4.1
	- Disponer de flex 2.6.0
	- Disponer de bison 3.0.4
	
- Navegar al directorio del proyecto desde terminal
- En caso de querer generar lex.yy.c a partir del fichero de especificación, ejecutar: flex --header-file=headers/lex.yy.h lex.l. Esto genera también el fichero .h. De todas formas, ya se ha incluido el fichero .c en la carpeta, por lo que este paso sería opcional.
- En caso de querer generar bison.tab.c a partir del fichero de especificación, ejecutar: bison bison.y  --defines=headers/bison.tab.h. Esto genera también el fichero .h. De todas formas, ya se ha incluido el fichero .c en la carpeta, por lo que este paso sería opcional.
- Ejecutar el comando make
- Ya se puede ejecutar la calculadora con ./ggg
- También se puede introducir como argumento un archivo para que lo lea la calculadora con ./ggg NombreArchivo
