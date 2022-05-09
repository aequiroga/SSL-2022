#include <stdio.h>
#include <ctype.h>

void imprimirChar(char x);

int main(int argc, char* argv[]) {
    
    char x = 0;
    //Si no hay argv, leo de consola y guardo en archivo
    if(argc<2) {
        FILE* file = fopen("output.txt","w");
        printf("Terminar ejecucion escribiendo un espacio\n");

        while(x!=' ') {
            if(x) imprimirChar(x);
            x = getchar();
            fwrite(&x,sizeof(char),1,file);
        }

        fclose(file);
    }

    //Si hay argv, leo del archivo e imprimo en consola
    else {
        FILE* file = fopen(argv[1],"r");
        FILE* file2 = fopen("output.txt","w");
        if (file==NULL) printf("No se encontro el archivo");
        else {
                printf("\nLeyendo del archivo %s \n\n", argv[1]);
                while(!feof(file)) {
                    if(x)imprimirChar(x);
                    fread(&x,sizeof(char),1,file);
                    fwrite(&x,sizeof(char),1,file2);
                }              
        }
        fclose(file);
        fclose(file2);
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
