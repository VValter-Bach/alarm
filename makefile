CC=gcc
LINKER=-lpthread -lwiringPi -lm
FLAGS=-Wall -Wextra -O3
OBJ=obj/ls.o obj/digits.o obj/ti.o obj/main.o
SRC=ls.c digits.c ti.c main.c
alarm: $(OBJ)
	$(CC) $(FLAGS) -o bin/alarm $(OBJ) $(LINKER)

obj/ls.o: ls.c
	$(CC) $(FLAGS) -c ls.c -o obj/ls.o $(LINKER)

obj/digits.o: digits.c
	$(CC) $(FLAGS) -c digits.c -o obj/digits.o $(LINKER)

obj/ti.o: ti.c
	$(CC) $(FLAGS) -c ti.c -o obj/ti.o $(LINKER)

obj/main.o: main.c
	$(CC) $(FLAGS) -c main.c -o obj/main.o $(LINKER)

clean:
	rm obj/ls.o obj/digits.o obj/ti.o obj/main.o

run: alarm
	./bin/alarm

