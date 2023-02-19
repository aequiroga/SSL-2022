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

expresion :                        IDENTIFICADOR '=' expresion          {if(procesarAsignacion($1,$3,ASIGNACION)==0){$$ = mostrarValorIdentificador();} else YYERROR;}
                               |   IDENTIFICADOR "+=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONSUMA)==0){$$ = mostrarValorIdentificador();} else YYERROR;}
                               |   IDENTIFICADOR "-=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONRESTA)==0){$$ = mostrarValorIdentificador();} else YYERROR;}
                               |   IDENTIFICADOR "*=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONMULTIPLICACION)==0){$$ = mostrarValorIdentificador();} else YYERROR;}
                               |   IDENTIFICADOR "/=" expresion         {if(procesarAsignacion($1,$3,ASIGNACIONCONDIVISION)==0){$$ = mostrarValorIdentificador();} else YYERROR;}
                               |   expresion '+' expresion              {$$ = $1 + $3;}                  
                               |   expresion '-' expresion              {$$ = $1 - $3;}
                               |   expresion '*' expresion              {$$ = $1 * $3;}
                               |   expresion '/' expresion              {if($3!=0) $$ = $1 / $3; else{mensajeError(DIVISIONPORCERO,"");YYERROR;}}
                               |   '-' expresion %prec NEG              {$$ = -$2;}
                               |   expresion '^' expresion              {$$ = pow($1,$3);}                    
                               |   '('expresion')'                      {$$ = $2;}
                               |   IDENTIFICADOR'('expresion')'         {if(existeFuncion($1)==0){$$ = procesarFuncion();} else YYERROR;}
                               |   IDENTIFICADOR                        {if(validarIdentificador($1)==0) $$ = mostrarValorIdentificador($1); else YYERROR;}
                               |   NRO
                               ;
                                 
declaracion :                      VAR IDENTIFICADOR                    {if(declarar(VAR,$2,0)!=0)  YYERROR;}
                               |   VAR IDENTIFICADOR '=' expresion      {if(declarar(VAR,$2,$4)!=0) YYERROR;}
                               |   CTE IDENTIFICADOR '=' expresion      {if(declarar(CTE,$2,$4)!=0) YYERROR;}
                               ;
%%

int nerrlex = 0;

void yyerror(const char *s){
	puts(s);
	return;
}
