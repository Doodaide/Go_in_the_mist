TARGET = gitm

CC = gcc

CFLAGS     = -c -Wall -Wvla -Werror -O0 -g -std=c11
ASAN_FLAGS = -fsanitize=address
SRC        = gitm.c
OBJ        = $(SRC:.c=.o)

all:$(TARGET)

$(TARGET):$(OBJ)
	$(CC) $(ASAN_FLAGS) -o $@ $(OBJ)

.SUFFIXES: .c .o

.c.o:
	 $(CC) $(CFLAGS) $(ASAN_FLAGS) $<

run:
	./$(TARGET)

test:
	echo what are we testing?!

clean:
	rm -f *.o *.obj $(TARGET)