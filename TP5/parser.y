%code top{
	#include <stdio.h>
    #include <math.h>
	#include "scanner.h"
    #include "calculadora.h"
}

%code provides {
	void yyerror(const char *);
	extern int nerrlex;
}

%union {
   double num;
   char *pala1;
}

%define api.value.type{char *}

%defines "parser.h"					
%output "parser.c"

%start sesionDeCalculadora
%define parse.error verbose  

%token<num> NRO                                 
%token<pala1> IDENTIFICADOR VAR CTE SALIR
%type<num> expresion

%token MASIGUAL "+="
%token MENOSIGUAL "-="
%token PRODUCTOIGUAL "*="
%token DIFERENCIAIGUAL "/="

%right "+=" "-=" "*=" "/=" '='
%left '-' '+'
%left '*' '/'
%right '^'
%precedence NEG

%%     
sesionDeCalculadora :              %empty                               
                               |   sesionDeCalculadora linea 
                               ;
  
linea :                            expresion '\n'                       {printf("%.10g\n",$1);}
                               |   declaracion '\n'
                               |   error '\n'
                               |   SALIR '\n'                           {if (yynerrs || nerrlex) YYABORT; else YYACCEPT;}
                               |   '\n'                          
                               ; 

expresion :                        IDENTIFICADOR '=' expresion          {procesarAsignacion($1,$3,ASIGNACION)}
                               |   IDENTIFICADOR "+=" expresion         {procesarAsignacion($1,$3,ASIGNACIONCONSUMA)}
                               |   IDENTIFICADOR "-=" expresion         {procesarAsignacion($1,$3,ASIGNACIONCONRESTA)}
                               |   IDENTIFICADOR "*=" expresion         {procesarAsignacion($1,$3,ASIGNACIONCONMULTIPLICACION)}
                               |   IDENTIFICADOR "/=" expresion         {procesarAsignacion($1,$3,ASIGNACIONCONDIVISION)}                   
                               |   expresion '+' expresion              {$$ = $1 + $3;}                  
                               |   expresion '-' expresion              {$$ = $1 - $3;}
                               |   expresion '*' expresion              {$$ = $1 * $3;}
                               |   expresion '/' expresion              {$$ = $1 / $3;}
                               |   '-' expresion %prec NEG              {$$ = -$2;}
                               |   expresion '^' expresion              {$$ = pow($1,$3);}                    
                               |   '('expresion')'                      {$$ = $2;}
                               |   IDENTIFICADOR'('expresion')'         {procesarFuncion($1,$3);}
                               |   IDENTIFICADOR                        {mostrarValorIdentificador($1);}
                               |   NRO                                  {printf($1);}    
                               ;
                                 
declaracion :                      VAR IDENTIFICADOR                    {declarar(VAR,$2,0);}
                               |   VAR IDENTIFICADOR '=' expresion      {declarar(VAR,$2,$4);}
                               |   CTE IDENTIFICADOR '=' expresion      {declarar(CTE,$2,$4);}
                               ;
%%

int nerrlex = 0;

void yyerror(const char *s){
	puts(s);
	return;
}
