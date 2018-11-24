all: main

main: main.o text.o
	gcc main.o text.o -o main

main.o: main.c
	gcc -c main.c

text.o: text.c text_structs.h
	gcc -c text.c