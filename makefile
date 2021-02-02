
all: sand

sand: sand.c
	gcc -Wall -Werror -ggdb3 -o sand sand.c

clean:
	rm -f sand
