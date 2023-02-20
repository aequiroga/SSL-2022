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
   char* texto;
}
                                                                 
%defines "parser.h"					
%output "parser.c"

%start sesionDeCalculadora
%define parse.error verbose  

%token<num> NRO                                 
%type<num> expresion
%token<texto> IDENTIFICADOR VAR CTE SALIR

%token MASIGUAL "+="
%token MENOSIGUAL "-="
%token PRODUCTOIGUAL "*="
%token DIFERENCIAIGUAL "/="

%token VERDICCIONARIO "diccionario"

%right "+=" "-=" "*=" "/=" '='
%left '-' '+'
%left '*' '/'
%right '^'
%precedence NEG

%%     
sesionDeCalculadora :              %empty                               
                               |   sesionDeCalculadora linea 
                               ;
  
linea :                            VERDICCIONARIO                       {verEntradas();}
                               |   expresion '\n'                       {printf("%.2g\n",$1);}
                               |   declaracion '\n'
                               |   error '\n'
                               |   SALIR '\n'                           {if (yynerrs || nerrlex) YYABORT; else YYACCEPT;}
                               |   '\n'                          
                               ;

expresion :                        IDENTIFICADOR '=' expresion          {if(procesarAsignacion($1,$3,ASIGNACION)==1)$$ = mostrarValorIdentificador($1);else YYERROR;} 
                               |   IDENTIFICADOR "+=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONSUMA)==1)$$ = mostrarValorIdentificador($1);else YYERROR;} 
                               |   IDENTIFICADOR "-=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONRESTA)==1)$$ = mostrarValorIdentificador($1);else YYERROR;} 
                               |   IDENTIFICADOR "*=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONMULTIPLICACION)==1)$$ = mostrarValorIdentificador($1);else YYERROR;} 
                               |   IDENTIFICADOR "/=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONDIVISION)==1)$$ = mostrarValorIdentificador($1);else YYERROR;}                   
                               |   expresion '+' expresion              {$$ = $1 + $3;}                  
                               |   expresion '-' expresion              {$$ = $1 - $3;}
                               |   expresion '*' expresion              {$$ = $1 * $3;}
                               |   expresion '/' expresion              {$$ = $1 / $3;}
                               |   '-' expresion %prec NEG              {$$ = -$2;}
                               |   expresion '^' expresion              {$$ = pow($1,$3);}                    
                               |   '('expresion')'                      {$$ = $2;}
                               |   IDENTIFICADOR'('expresion')'         {if(existeFuncion($1)==1) $$ = procesarFuncion($1,$3);else YYERROR;}
                               |   IDENTIFICADOR                        {if(existeVarCte($1)==1) $$ = mostrarValorIdentificador($1);else YYERROR;}
                               |   NRO 
                               ;                                                        
declaracion :                      VAR IDENTIFICADOR                    {if(declarar($2,1,0)!=0)YYERROR;}
                               |   VAR IDENTIFICADOR '=' expresion      {if(declarar($2,1,$4)!=0)YYERROR;}
                               |   CTE IDENTIFICADOR '=' expresion      {if(declarar($2,2,$4)!=0)YYERROR;}
                               |   CTE IDENTIFICADOR                    {mensajeDeError(DECLARACTESINVALOR,$2);YYERROR;}
                               ;
%%

int nerrlex = 0;

void yyerror(const char *s){
	puts(s);
	return;
}

