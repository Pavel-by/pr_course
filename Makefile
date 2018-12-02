all: main

main: main.o text.o course_utils.o interface.o
	gcc main.o text.o course_utils.o interface.o -o main

main.o: main.c text_structs.h
	gcc -c main.c

text.o: text.c text_structs.h
	gcc -c text.c

course_utils.o: course_utils.c text_structs.h
	gcc -c course_utils.c

interface.o: interface.c
	gcc -c interface.c

clear:
	rm *.o

run:
	./main