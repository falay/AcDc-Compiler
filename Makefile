CC = gcc
CCFLAGS = -lm -O2 -std=c99
MOVE = $(shell mv *.o src)

all: src/AcDc.o src/parser.o src/scanner.o src/tools.o src/symtable.o src/generator.o
	${CC} ${CCFLAGS} $^ -o AcDc
	echo $(MOVE)	
	
src/%.o: src/%.c
	${CC} $< -c

clean:
	rm -rf AcDc src/*.o	