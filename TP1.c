#include <stdio.h>
#include <ctype.h>

void imprimirChar(char x);

int main(int argc, char* argv[]) {

    char x = 0;
    if(argc<2) {//Si no hay argv, leo de consola
        printf("Terminar ejecucion con Ctrl+C\n");
        while(1) {
            if(x)imprimirChar(x);
            x = getchar();
        }
    }
    else {
        FILE* file = fopen(argv[1],"r");

        if(file==NULL) printf("No se encontro el archivo");
        else while(!feof(file)) {//Si hay argv, leo del archivo
            if(x)imprimirChar(x);
            fread(&x,sizeof(char),1,file);
        }
    }
    return 0;
}

    void imprimirChar(char x) {
        if(isalpha(x))
        {
            if(islower(x)) putchar (toupper(x));
            if(isupper(x)) putchar (tolower(x));
        }
        else if(!isdigit(x)) putchar(x);
    }