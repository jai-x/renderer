EXE = renderer
LIBS = -lm -lSDL2
CFLAGS = -std=c99 -Wall -Wextra -pedantic -pedantic-errors

SRCS = $(wildcard src/*.c)
HDRS = $(wildcard src/*.h)

$(EXE): $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) $(LIBS) $(SRCS) -o $(EXE)

clean:
	$(RM) $(EXE)
