CC = clang
CFLAGS = -Wall -Werror -O3 -g
EXEC = a.out

all: $(EXEC)

$(EXEC): $(C:.c=.o) bmp_reader.o
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $^ -o $@ $(CFLAGS)

clean:
	rm $(EXEC) *.o

.PHONY: clean
