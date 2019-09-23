
PROGRAM = catalan
FILES.c = main.c catalan.c
FILES.h = catalan.h
FILES.o = $(FILES.c:.c=.o)
FLAGS = -pthread -c -o


all: $(PROGRAM)

$(PROGRAM): $(FILES.o)
	gcc -pthread -o $@ $(FILES.o)

egonzalez_prog2.o: main.c $(FILES.h)
	gcc $(FLAGS) main.o main.c

catalan.o: catalan.c $(FILES.h)
	gcc $(FLAGS) catalan.o catalan.c

clean:
	rm *.o; rm *.dat; rm $(PROGRAM)

cleandat:
	rm *.dat
