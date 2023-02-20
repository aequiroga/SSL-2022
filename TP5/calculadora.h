struct symrec {
    char* nombre;
    int tipo;
    union {
        double valorNumero;
        double (*funcion)(double);
    } valor;  
};

struct funciones {
    char* nombre;
    int tipo;
    double (*funcion)(double);
};

enum tipoEntrada
{
    VARIABLE = 1,
    CONSTANTE,
    FUNCION
};

enum tipoError
{
    NODECLARADO = 1,
    ASIGNACIONNOVAR,
    DICCIONARIOSINESPACIO,
    NOESFUNCION,
    ESFUNCION,
    YADECLARADO,
    ASIGNACIONDIVISIONPORCERO,
    DIVISIONPORCERO,
    DECLARACTESINVALOR,
    ERRORDESCONOCIDO
};

enum tipoOperacionAsignacion
{
    ASIGNACION = 1,
    ASIGNACIONCONSUMA,
    ASIGNACIONCONRESTA,
    ASIGNACIONCONMULTIPLICACION,
    ASIGNACIONCONDIVISION
};

int insertarFuncionDiccionario(char*, int, double (*funcion)(double));
int insertarEntradaDiccionario(char*, int, double);
int declarar(char*, int, double);
int existeVarCte(char*);
int existeFuncion(char *);
double procesarFuncion(char*, double);
int procesarAsignacion(char *, double , int);
double asignar(int, int, double);
double mostrarValorIdentificador(char*);
int buscarEntradaDiccionario(char*) ;
int inicializarDiccionario();
void mensajeDeError(int, char *);
void verEntradas();