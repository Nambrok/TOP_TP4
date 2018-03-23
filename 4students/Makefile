CFLAGS = -Werror -Wall -O3
LIBNAME = libbmp_reader.so

all: $(LIBNAME)

$(LIBNAME): bmp_reader.c
	$(CC) -c $^ -o bmp_reader.o $(CFLAGS) -fPIC
	$(CC) -shared bmp_reader.o -o $@

clean:
	rm *.o $(LIBNAME)

.PHONY: clean
