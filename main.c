#include <stdio.h>
#include "parser.h"
#include "calculadora.h"

int main() {
	
	inicializarTabla();

	switch( yyparse() ){
	case 0:
		puts("Pertenece al LIC"); return 0;
	case 1:
		puts("No pertenece al LIC"); return 1;
	case 2:
		puts("Memoria insuficiente"); return 2;
	}
	return 0;
}