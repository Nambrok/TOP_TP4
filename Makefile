CC = mpicc
CFLAGS = -Wall -Werror -O3 -g
NPROC = 2
EXEC = a.out

all: $(EXEC)

$(EXEC): $(C:.c=.o)
	$(CC) $^ -o $@ -L. -lbmp_reader

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS) -I.

run: $(EXEC)
	mpiexec -n $(NPROC) ./$(EXEC)	
	gwenview copie.bmp

clean:
	rm $(EXEC) *.o

.PHONY: clean
