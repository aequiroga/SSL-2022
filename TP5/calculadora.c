#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "parser.h"
#include "calculadora.h"

#define MAXIMOENTRADASDICCIONARIO 200
#define SIZEMENSAJEERROR 200

struct funciones funciones[8] =
    {
        {"sin", 3, sin},
        {"cos", 3, cos},
        {"tan", 3, tan},
        {"asin", 3, asin},
        {"acos", 3, acos},
        {"atan", 3, atan},
        {"sqrt", 3, sqrt},
        {"log", 3, log}};

struct symrec diccionario[MAXIMOENTRADASDICCIONARIO];
int posicionDiccionario = 0;
char mensajeError[SIZEMENSAJEERROR];
char* stringVacio = "";

void mensajeDeError(int tipoError, char *nombreEntrada)

{
    switch (tipoError)

    {

    case NODECLARADO:

        sprintf(mensajeError, "La variable/constante %s no ha sido declarada.", nombreEntrada);

        yyerror(mensajeError);

        break;

    case ASIGNACIONNOVAR:

        int posicionEntrada = buscarEntradaDiccionario(nombreEntrada);

        if (diccionario[posicionEntrada].tipo == CONSTANTE)

            sprintf(mensajeError, "Los operadores de asignación solo admiten una variable como operando izquierdo. La entrada %s es de tipo constante", nombreEntrada);

        else

            sprintf(mensajeError, "Los operadores de asignación solo admiten una variable como operando izquierdo. La entrada %s es una funcion", nombreEntrada);

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
    case DECLARACTESINVALOR:

        sprintf(mensajeError, "Para declarar una constante se debe especificar un valor");

        yyerror(mensajeError);

        break;

    default:

        sprintf(mensajeError, "Error desconocido.");

        yyerror(mensajeError);

        break;
    }
}

int insertarFuncionDiccionario(char *nombreEntrada, int tipoEntrada, double (*funcion)(double))
{
    if (posicionDiccionario < MAXIMOENTRADASDICCIONARIO)
    {
        diccionario[posicionDiccionario].nombre = nombreEntrada;
        diccionario[posicionDiccionario].tipo = tipoEntrada;
        diccionario[posicionDiccionario].valor.funcion = funcion;
        posicionDiccionario++;
        return 0;
    }
    mensajeDeError(DICCIONARIOSINESPACIO, stringVacio);
    return -1;
}

int insertarEntradaDiccionario(char *nombreEntrada, int tipoEntrada, double valorEntrada)
{
    if (posicionDiccionario < MAXIMOENTRADASDICCIONARIO)
    {
        diccionario[posicionDiccionario].nombre = nombreEntrada;
        diccionario[posicionDiccionario].tipo = tipoEntrada;
        diccionario[posicionDiccionario].valor.valorNumero = valorEntrada;
        printf("INserto entrada %s de tipo %d \n\n",nombreEntrada,tipoEntrada);
        posicionDiccionario++;
        return 0;
    }
    mensajeDeError(DICCIONARIOSINESPACIO, stringVacio);
    return -1;
}


int buscarEntradaDiccionario(char *nombreEntrada)
{
    for (int i = 0; i < posicionDiccionario; i++)
    {
        if(diccionario[i].nombre!=NULL)
        {
            if (strcmp(diccionario[i].nombre, nombreEntrada) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

int declarar(char *nombreEntrada, int tipoEntrada, double valorEntrada)
{
    int posicionEntrada = buscarEntradaDiccionario(nombreEntrada);
    if(posicionEntrada==-1) {

    if (tipoEntrada != FUNCION)
    {
        insertarEntradaDiccionario(nombreEntrada, tipoEntrada, valorEntrada);
        return 0;
    }   
        mensajeDeError(ESFUNCION, nombreEntrada);
    }
    else mensajeDeError(YADECLARADO,nombreEntrada);
    return -1;
}

int existeVarCte(char *nombreEntrada)
{
    int posicionEntrada = buscarEntradaDiccionario(nombreEntrada);
    if (posicionEntrada >= 0)
    {
        if (diccionario[posicionEntrada].tipo == FUNCION){
            mensajeDeError(ESFUNCION,nombreEntrada);
            return -1;
        }
        return 1;
    }
    mensajeDeError(NODECLARADO,nombreEntrada);
    return -1;
}

int existeFuncion(char *nombreFuncion)

{

    int posicionEntrada = buscarEntradaDiccionario(nombreFuncion);

    if (posicionEntrada >= 0)

    {
        int tipoDeEntrada = diccionario[posicionDiccionario].tipo;
        if (tipoDeEntrada == FUNCION)

        {
            return 1;
        }

        mensajeDeError(NOESFUNCION,nombreFuncion);
    }
    else mensajeDeError(NODECLARADO,nombreFuncion);

    return -1;
}

double procesarFuncion(char *nombreFuncion, double valor)

{
    int posicionEntrada = buscarEntradaDiccionario(nombreFuncion);
    return posicionEntrada > -1 ? diccionario[posicionEntrada].valor.funcion(valor) : -1;
}

int procesarAsignacion(char *nombreEntrada, double valorParaAsignar, int tipoOperacion)

{
    int posicionEntrada = buscarEntradaDiccionario(nombreEntrada);

    if (posicionEntrada >= 0)

    {
        if (diccionario[posicionEntrada].tipo == VARIABLE)
        {
            if (asignar(posicionEntrada, tipoOperacion, valorParaAsignar))
                return 1;
            mensajeDeError(ERRORDESCONOCIDO,stringVacio);
            return -1;
        }

        mensajeDeError(ASIGNACIONNOVAR, nombreEntrada);
    }

    else

        mensajeDeError(NODECLARADO, nombreEntrada);

    return -1;
}

double asignar(int posicionEntradaDiccionario, int tipoOperacionAsignacion, double valorParaAsignar)

{

    switch (tipoOperacionAsignacion)

    {

    case ASIGNACION:

        diccionario[posicionEntradaDiccionario].valor.valorNumero = valorParaAsignar;

        break;

    case ASIGNACIONCONSUMA:

        diccionario[posicionEntradaDiccionario].valor.valorNumero += valorParaAsignar;

        break;

    case ASIGNACIONCONRESTA:

        diccionario[posicionEntradaDiccionario].valor.valorNumero -= valorParaAsignar;

        break;

    case ASIGNACIONCONMULTIPLICACION:

        diccionario[posicionEntradaDiccionario].valor.valorNumero *= valorParaAsignar;

        break;

    case ASIGNACIONCONDIVISION:

        if (valorParaAsignar != 0)

            diccionario[posicionEntradaDiccionario].valor.valorNumero /= valorParaAsignar;

        else

        {

            // mensajeError(ASIGNACIONDIVISIONPORCERO, diccionario[posicionEntradaDiccionario].nombre);

            return -1;
        }

        break;
    }

    return 1;
}

double mostrarValorIdentificador(char *nombreEntrada)
{
    int posicionEntrada = buscarEntradaDiccionario(nombreEntrada);
    return diccionario[posicionEntrada].valor.valorNumero;
}

int inicializarDiccionario()
{
    for (int i = 0; i < 8; i++)
    {
        if (insertarFuncionDiccionario(funciones[i].nombre, FUNCION, funciones[i].funcion) == -1){
            mensajeDeError(ERRORDESCONOCIDO,stringVacio);
            return -1;
        }
    }
    if (insertarEntradaDiccionario("pi", CONSTANTE, 3.141592654) == -1)
        return -1;
    if (insertarEntradaDiccionario("e", CONSTANTE, 2.718281828) == -1)
        return -1;
    return 0;
}

void verEntradas() {
    for(int i =0; i < posicionDiccionario; i++) {
        printf("Tipo: %d, Nombre = %s",diccionario[i].tipo,diccionario[i].nombre);
        printf("\n");
    }
}