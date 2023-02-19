#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <calculadora.h>
#include "parser.h"
#define TAMANODICCIONARIO 200
#define TAMANOMENSAJEERROR 200

struct funciones arrayFunciones[8] =
    {
        {"sin", FUNCION, sin},
        {"cos", FUNCION, cos},
        {"tan", FUNCION, tan},
        {"asin", FUNCION, asin},
        {"acos", FUNCION, acos},
        {"atan", FUNCION, atan},
        {"sqrt", FUNCION, sqrt},
        {"log", FUNCION, log},
};

symrec diccionario[TAMANODICCIONARIO]; // Diccionario
int posicionDiccionario = 0;           // Indice para agregar elementos a diccionario

void agregarEntradaDiccionario(char *nombre, tipoEntrada tipoEntrada, double valorEntrada)
{
   posicionDiccionario++;
   diccionario[posicionDiccionario].nombre = nombre;
   diccionario[posicionDiccionario].tipo = tipoEntrada;
   diccionario[posicionDiccionario].valor.nro = valorEntrada;
}

int tipoEntradaCorrecto(tipoEntrada tipoDeEntrada, tipoEntrada tipoDeEntradaDeseado)
{
   return tipoDeEntrada == tipoDeEntradaDeseado ? 0 : -1;
}

// Valido tipos y existencia de entrada, y llamo a la asignacion. Si falla no asigna.
void procesarAsignacion(char *nombreEntrada, double valorParaAsignar, tipoOperacionAsignacion tipoOperacion)
{
   int posicionDiccionario = buscarEntradaDiccionario(nombreEntrada);
   tipoEntrada tipoDeEntrada = diccionario[posicionDiccionario].tipo;
   if (posicionDiccionario >= 0)
   {
      if (tipoEntradaCorrecto(tipoDeEntrada, VAR) == 0)
         asignar(nombreEntrada, tipoOperacion, valorParaAsignar, posicionDiccionario);
      else
         mensajeError(ASIGNACIONNOVAR, "");
   }
   else
      mensajeError(NODECLARADO, nombreEntrada);
}

// Valido tipos y existencia de entrada, y devuelvo el resultado de aplicar la funcion al valor
double procesarFuncion(char *nombreFuncion, double valorParaFuncion)
{
   int posicionDiccionario = buscarEntradaDiccionario(nombreFuncion);
   tipoEntrada tipoDeEntrada = diccionario[posicionDiccionario].tipo;
   if (posicionDiccionario >= 0)
   {
      if (tipoEntradaCorrecto(tipoDeEntrada, FUNCION) == 0)
         return diccionario[posicionDiccionario].valor.funcion(diccionario[posicionDiccionario].valor.nro);
      else
         mensajeError(NOESFUNCION, nombreFuncion);
   }
}

void mostrarValorIdentificador(char *nombreEntrada)
{
   int posicionDiccionario = buscarEntradaDiccionario(nombreEntrada);
   tipoEntrada tipoDeEntrada = diccionario[posicionDiccionario].tipo;
   if (posicionDiccionario >= 0)
   {
      if (tipoEntradaCorrecto(tipoDeEntrada, FUNCION) != 0) // Invierto la logica para validar que no sea funcion
         return diccionario[posicionDiccionario].valor.nro;
      else
      {
         mensajeError(ESFUNCION, nombreEntrada);
      }
   }
}

void declarar(tipoEntrada tipoDeEntrada, char *nombreEntrada, double valorParaAsignar)
{
   int posicionDiccionario = buscarEntradaDiccionario(nombreEntrada);
   if (posicionDiccionario == -1) // Validar que no existe la entrada
   {
      agregarEntradaDiccionario(nombreEntrada, tipoDeEntrada, valorParaAsignar);
   }
   else
      mensajeError(YADECLARADO, nombreEntrada);
}

// Busco entrada en diccionario y asigno posicion a variable, si no la encuentra devuelve -1
int buscarEntradaDiccionario(char *nombre)
{
   for (int i = 0; i < posicionDiccionario; i++)
   {
      if (strcmp(diccionario[i].nombre, nombre))
         return i;
   }
   return -1;
}

void mensajeError(tipoError tipoError, char *nombreEntrada)
{
   char *mensajeError = malloc(sizeof(char) * TAMANOMENSAJEERROR);
   switch (tipoError)
   {
   case NODECLARADO:
      sprintf(mensajeError, "La variable/constante %s no ha sido declarada.", nombreEntrada);
      yyerror(mensajeError);
      break;
   case ASIGNACIONNOVAR:
      sprintf(mensajeError, "Los operadores de asignación solo admiten una variable como operando izquierdo.");
      yyerror(mensajeError);
      break;
   case DICCIONARIOSINESPACIO:
      sprintf(mensajeError, "Se alcanzo el tamaño maximo del diccionario, no es posible declarar nuevas entradas.");
      yyerror(mensajeError);
      break;
   case NOESFUNCION:
      sprintf(mensajeError, "La entrada %s no es una funcion existente.", nombreEntrada);
      yyerror(mensajeError);
      break;
   case ESFUNCION:
      sprintf(mensajeError, "La entrada %s es una funcion.", nombreEntrada);
      yyerror(mensajeError);
      break;
   case YADECLARADO:
      sprintf(mensajeError, "La entrada %s ya fue declarada en el diccionario.", nombreEntrada);
      yyerror(mensajeError);
      break;
   default:
      sprintf(mensajeError, "Error desconocido.", nombreEntrada);
      yyerror(mensajeError);
      break;
   }
}

double asignar(int posicionEntradaDiccionario, tipoOperacionAsignacion tipoOperacionAsignacion, double valorParaAsignar)
{
   switch (tipoOperacionAsignacion)
   {
   case ASIGNACION:
      diccionario[posicionEntradaDiccionario].valor.nro = valorParaAsignar;
      break;
   case ASIGNACIONCONSUMA:
      diccionario[posicionEntradaDiccionario].valor.nro += valorParaAsignar;
      break;
   case ASIGNACIONCONRESTA:
      diccionario[posicionEntradaDiccionario].valor.nro -= valorParaAsignar;
      break;
   case ASIGNACIONCONMULTIPLICACION:
      diccionario[posicionEntradaDiccionario].valor.nro *= valorParaAsignar;
      break;
   case ASIGNACIONCONDIVISION:
      if (valorParaAsignar > 0)
      {
         diccionario[posicionEntradaDiccionario].valor.nro /= valorParaAsignar;
      }
      break;
   }
   return diccionario[posicionEntradaDiccionario].valor.nro;
}

// Inserto funciones y constantes iniciales en el diccionario
void inicializarTabla()
{
   // Se agregan validaciones de espacio para el caso hipotetico que el diccionario no posea suficiente espacio para las entradas iniciales
   for (int i = 0; i < 8; i++)
   {
      if (posicionDiccionario != TAMANODICCIONARIO)
      {
         diccionario[i].nombre = arrayFunciones[i].nombre;
         diccionario[i].tipo = arrayFunciones[i].tipo;
         diccionario[i].valor.funcion = arrayFunciones[i].funcion;
         posicionDiccionario++;
      }
      else
         mensajeError(DICCIONARIOSINESPACIO, "");
   }

   switch (TAMANODICCIONARIO - posicionDiccionario)
   {
   case 0:
      mensajeError(DICCIONARIOSINESPACIO, "");
      break;
   case 1:
      agregarEntradaDiccionario("pi", CTE, 3.141592654);
      mensajeError(DICCIONARIOSINESPACIO, "");
      break;
   default:
      agregarEntradaDiccionario("pi", CTE, 3.141592654);
      agregarEntradaDiccionario("e", CTE, 2.718281828);
      break;
   }
}