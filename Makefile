#generamos el ejecutable calculadora - make
calculadora : main.o calculadora.o parser.o scanner.o
	gcc -o calculadora main.o calculadora.o parser.o scanner.o -lfl -lm

#generamos el objeto de main
main.o: main.c parser.h
	gcc -c main.c

#generamos el objeto de calculadora
calculadora.o: calculadora.c calculadora.h
	gcc -c calculadora.c

#generamos el objeto del parser
parser.o: parser.c parser.h
	gcc -c parser.c

parser.c parser.h: parser.y scanner.h calculadora.h
	bison parser.y

#generamos el objeto del scanner
scanner.o: scanner.c scanner.h
	gcc -c scanner.c	

scanner.c scanner.h: lexico.l
	flex lexico.l

#eliminamos todo lo generado - make clean
clean:
	rm scanner.c scanner.h scanner.o parser.c parser.h parser.o calculadora.o main.o calculadora
