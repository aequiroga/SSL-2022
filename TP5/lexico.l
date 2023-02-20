%{
   #include <stdio.h>
   #include "parser.h"
%}

%{
    char msg[120];
%}

%option noinput
%option nounput
%option outfile="scanner.c" 
%option header-file="scanner.h" 

letra          [a-zA-Z]
digito         [0-9]  
expo           [eE][+-]?{digito}+
fracc          {digito}*\.{digito}+|{digito}+\.{digito}*
letra_Digito   [a-zA-Z0-9]
entero         {digito}+
real           {fracc}+{expo}?|{digito}+{expo}
numeros        {entero}|{real}
otros          [[:punct:]]{-}[+\-*/=^()]
espacios       [[:space:]]{-}[\n]

%%
"diccionario"                                               return VERDICCIONARIO;
var                                                         return VAR;
cte                                                         return CTE;
salir                                                       return SALIR;
{letra}{letra_Digito}*                                      {yylval.texto = strdup(yytext);return IDENTIFICADOR;}                                                           
{numeros}                                                   {sscanf(yytext,"%lf",&yylval.num);return NRO;}  
"+="                                                        return MASIGUAL;
"-="                                                        return MENOSIGUAL;
"*="                                                        return PRODUCTOIGUAL;
"/="                                                        return DIFERENCIAIGUAL;
"+"                                                         return '+';
"-"                                                         return '-';
"*"                                                         return '*';
"/"                                                         return '/';
"^"                                                         return '^';
"="                                                         return '=';
"("                                                         return '(';
")"                                                         return ')';
"\n"                                                        return '\n';

{letra}{letra_Digito}*{otros}+({letra_Digito}|{otros})*     {nerrlex++; sprintf(msg,"Error lexico: identificador invalido: %s",yytext); yyerror(msg);}
{entero}+{letra}+{letra_Digito}*                            {nerrlex++; sprintf(msg,"Error lexico: constante invalida: %s",yytext); yyerror(msg);}
{real}+{letra}+{real}*                                      {nerrlex++; sprintf(msg,"Error lexico: Real invalida: %s",yytext); yyerror(msg);}
{otros}+                                                    {nerrlex++; sprintf(msg,"Error lexico: cadena desconocida: %s",yytext); yyerror(msg);}
{espacios}                                                  ;
%%


