OBJ = main.o coder.o command.o
FLAG = -Wall -Werror -g -O0
DEPS = -MMD

all: main

main: $(OBJ)
	gcc -o main $(OBJ)
    
main.o: src/main.c
	gcc $(FLAG) -c src/main.c

coder.o: src/coder.c
	gcc $(FLAG) -c src/coder.c
	
command.o: src/command.c
	gcc $(FLAG) -c src/command.c
		  
clean:
	rm main *.o

-include main.d coder.d command.d

.PHONY: all clean