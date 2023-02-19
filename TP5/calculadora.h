enum tipoEntrada
{
    VARIABLE = 1,
    CONSTANTE,
    FUNCION
};
enum tipoOperacionAsignacion
{
    ASIGNACION = 1,
    ASIGNACIONCONSUMA,
    ASIGNACIONCONRESTA,
    ASIGNACIONCONMULTIPLICACION,
    ASIGNACIONCONDIVISION
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
    DIVISIONPORCERO
};

typedef struct symrec symrec;
typedef enum tipoEntrada tipoEntrada;
typedef enum tipoOperacionAsignacion tipoOperacionAsignacion;
typedef enum tipoError tipoError;

struct funciones
{
    char *nombre;
    int tipo;
    double (*funcion)(double);
};

struct symrec
{
    char *nombre;
    tipoEntrada tipo;
    union
    {
        double nro;
        double (*funcion)(double);
    } valor;
};

int declarar(tipoEntrada, char *, double);                       // DONE
int procesarAsignacion(char *, double, tipoOperacionAsignacion);  // DONE
int existeFuncion(char *nombreFuncion);
int validarIdentificador(char* nombreEntrada);
double procesarFuncion();                                         // DONE
double mostrarValorIdentificador();                            // DONE
void agregarEntradaDiccionario(char *, enum tipoEntrada, double); // DONE
double asignar(int, enum tipoOperacionAsignacion, double);        // DONE
int buscarEntradaDiccionario(char *);                             // DONE
void inicializarTabla();                                          // DONE