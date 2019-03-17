EXE = renderer
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic -pedantic-errors

all: $(EXE)

$(EXE): main.c
	$(CC) $(CFLAGS) main.c -o $(EXE)

clean:
	$(RM) $(EXE)
