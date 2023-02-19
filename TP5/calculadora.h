struct funciones
{
    char *nombre;
    int tipo;
    double (*funcion)(double);
};

struct symrec
{
    char *nombre;     // guardo una cadena
    tipoEntrada tipo; // tipo 1-VAR 2-CTE 3-FUNCION
    union
    {
        double nro;
        double (*funcion)(double); // puntero a funcion que recibe 1 parametro tipo double y devuelve un double
    } valor;
};

enum tipoEntrada
{
    VAR = 1,
    CTE,
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
    YADECLARADO
};

typedef struct symrec symrec;
typedef enum tipoEntrada tipoEntrada;
typedef enum tipoOperacionAsignacion tipoOperacionAsignacion;
typedef enum tipoError tipoError;

void declarar(tipoEntrada, char *, double);                       // DONE
void procesarAsignacion(char *, double, tipoOperacionAsignacion); // DONE
double procesarFuncion(char *, double);                           // DONE
void mostrarValorIdentificador(char *);                           // DONE
void agregarEntradaDiccionario(char *, enum tipoEntrada, double); // DONE
double asignar(int, enum tipoOperacionAsignacion, double);     // DONE
int buscarEntradaDiccionario(char *);                             // DONE
tipoEntrada tipoIdentificador(char *);                            //???
void inicializarTabla();                                          // DONE