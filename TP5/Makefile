#generamos el ejecutable calculadora - make
calculadora : main.o calc.o parser.o scanner.o
	gcc -o calculadora main.o calc.o parser.o scanner.o -lfl -lm

#generamos el objeto de main
main.o: main.c parser.h
	gcc -c main.c

#generamos el objeto de calc
calc.o: calc.c calc.h
	gcc -c calc.c

#generamos el objeto del parser
parser.o: parser.c parser.h
	gcc -c parser.c

parser.c parser.h: parser.y scanner.h calc.h
	bison parser.y

#generamos el objeto del scanner
scanner.o: scanner.c scanner.h
	gcc -c scanner.c	

scanner.c scanner.h: lexico.l
	flex lexico.l

#eliminamos todo lo generado - make clean
clean:
	rm scanner.c scanner.h scanner.o parser.c parser.h parser.o calc.o main.o calculadora
