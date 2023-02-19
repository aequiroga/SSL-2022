#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "calculadora.h"
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
int ultimoIndiceBuscado = -1;          // Variable a forma de cache, se le asigna -1 para indicar que no posee registro

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
int procesarAsignacion(char *nombreEntrada, double valorParaAsignar, tipoOperacionAsignacion tipoOperacion)
{
   ultimoIndiceBuscado = buscarEntradaDiccionario(nombreEntrada);
   tipoEntrada tipoDeEntrada = diccionario[posicionDiccionario].tipo;

   if (ultimoIndiceBuscado >= 0)
   {
      if (tipoEntradaCorrecto(tipoDeEntrada, VARIABLE) == 0)
      {
         if (asignar(ultimoIndiceBuscado, tipoOperacion, valorParaAsignar) == 0)
            return 0;
         return -1;
      }
      mensajeError(ASIGNACIONNOVAR, NULL);
   }
   else
      mensajeError(NODECLARADO, nombreEntrada);
   return -1;
}

// Validaciones de tipo y existencia de funcion, en caso de error devuelve -1
int existeFuncion(char *nombreFuncion)
{
   ultimoIndiceBuscado = buscarEntradaDiccionario(nombreFuncion);
   tipoEntrada tipoDeEntrada = diccionario[posicionDiccionario].tipo;
   if (ultimoIndiceBuscado >= 0)
   {
      if (tipoEntradaCorrecto(tipoDeEntrada, FUNCION) == 0)
      {
         return 0;
      }
      mensajeError(NOESFUNCION, nombreFuncion);
   }
   return -1;
}

// Devuelvo el resultado de aplicar la funcion al valor
double procesarFuncion()
{
   return ultimoIndiceBuscado > -1 ? diccionario[ultimoIndiceBuscado].valor.funcion(diccionario[ultimoIndiceBuscado].valor.nro) : -1;
}

// Valida que un identificador sea de tipo VAR/CTE y que exista
int validarIdentificador(char *nombreEntrada)
{
   int posicionDiccionario = buscarEntradaDiccionario(nombreEntrada);
   tipoEntrada tipoDeEntrada = diccionario[posicionDiccionario].tipo;

   // Si la entrada es valida guardo posicion en cache, sino -1
   if (posicionDiccionario > 0)
   {
      if (tipoDeEntrada != FUNCION)
      {
         ultimoIndiceBuscado = posicionDiccionario;
         return 0;
      }
      mensajeError(ESFUNCION, nombreEntrada);
   }
   else
      mensajeError(NODECLARADO, nombreEntrada);
   return -1;
}

// Muestra el valor de una VAR/CTE
double mostrarValorIdentificador()
{
   return diccionario[ultimoIndiceBuscado].valor.nro;
}

int declarar(tipoEntrada tipoDeEntrada, char *nombreEntrada, double valorParaAsignar)
{
   if (posicionDiccionario != TAMANODICCIONARIO - 1)
   {
      ultimoIndiceBuscado = buscarEntradaDiccionario(nombreEntrada);
      if (ultimoIndiceBuscado == -1) // Validar que no existe la entrada
      {
         agregarEntradaDiccionario(nombreEntrada, tipoDeEntrada, valorParaAsignar);
         return 0;
      }
      mensajeError(YADECLARADO, nombreEntrada);
   }
   else
      mensajeError(DICCIONARIOSINESPACIO, *"");
   return -1;
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

void mensajeError(tipoError tipoError, char* nombreEntrada)
{
   char *mensajeError = malloc(sizeof(char) * TAMANOMENSAJEERROR);
   switch (tipoError)
   {
   case NODECLARADO:
      sprintf(mensajeError, "La variable/constante %s no ha sido declarada.", nombreEntrada);
      yyerror(mensajeError);
      break;
   case ASIGNACIONNOVAR:
      tipoEntrada tipoDeEntrada = diccionario[ultimoIndiceBuscado].tipo;
      if (tipoDeEntrada == CONSTANTE)
         sprintf(mensajeError, "Los operadores de asignación solo admiten una variable como operando izquierdo. La entrada %s es de tipo constante");
      else
         sprintf(mensajeError, "Los operadores de asignación solo admiten una variable como operando izquierdo. La entrada %s es una funcion");
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
   case ASIGNACIONDIVISIONPORCERO:
   case DIVISIONPORCERO:
      sprintf(mensajeError, "No es posible dividir por cero");
      yyerror(mensajeError);
      break;
   default:
      sprintf(mensajeError, "Error desconocido.");
      yyerror(mensajeError);
      break;
   }
}

// Asigna un valor a una entrada de diccionario, en caso de error devuelve -1
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
      if (valorParaAsignar != 0)
         diccionario[posicionEntradaDiccionario].valor.nro /= valorParaAsignar;
      else
      {
         mensajeError(ASIGNACIONDIVISIONPORCERO, NULL);
         return -1;
      }
      break;
   }
   return 0;
}

// Inserto funciones y constantes iniciales en el diccionario
void inicializarTabla()
{
   // Se agregan validaciones de espacio para el caso hipotetico que el diccionario no posea suficiente espacio para las entradas iniciales
   for (int i = 0; i < 8; i++)
   {
      if (posicionDiccionario != TAMANODICCIONARIO - 1)
      {
         diccionario[i].nombre = arrayFunciones[i].nombre;
         diccionario[i].tipo = arrayFunciones[i].tipo;
         diccionario[i].valor.funcion = arrayFunciones[i].funcion;
         posicionDiccionario++;
      }
      else
         mensajeError(DICCIONARIOSINESPACIO, NULL);
   }

   switch (TAMANODICCIONARIO - posicionDiccionario - 1)
   {
   case 0:
      mensajeError(DICCIONARIOSINESPACIO, NULL);
      break;
   case 1:
      agregarEntradaDiccionario("pi", CONSTANTE, 3.141592654);
      mensajeError(DICCIONARIOSINESPACIO, NULL);
      break;
   default:
      agregarEntradaDiccionario("pi", CONSTANTE, 3.141592654);
      agregarEntradaDiccionario("e", CONSTANTE, 2.718281828);
      break;
   }
}