#include <stdio.h>
#include "calculadora.h"
#include "parser.h"

int main() {

	if(inicializarDiccionario()<0){
		printf("Error al inicializar el diccionario");
		return -1;
	}
	//printf("Inicializacion exitosa \n");
	switch( yyparse() ){
	case 0:
		puts("Hasta la proxima!"); return 0;
	case 1:
		puts("No pertenece al LIC"); return 1;
	case 2:
		puts("Memoria insuficiente"); return 2;
	}
	return 0;
}