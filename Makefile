EXE = renderer
LIBS = -lm -lSDL2
CFLAGS = -std=c99 -Wall -Wextra -pedantic -pedantic-errors

SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)

$(EXE): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(LIBS) $(SRCS) -o $(EXE)

clean:
	$(RM) $(EXE)
