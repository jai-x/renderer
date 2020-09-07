.PHONY: clean

EXE = renderer
LIBS = -lm -lSDL2
CFLAGS = -std=c99 -Wall -Wextra -Werror -Wpedantic -pedantic-errors -pipe

SRC_DIR = src
BUILD_DIR = build

OBJS = \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/face.o \
	$(BUILD_DIR)/model.o \
	$(BUILD_DIR)/screen.o \
	$(BUILD_DIR)/screen_draw.o \
	$(BUILD_DIR)/util.o \
	$(BUILD_DIR)/vec2i.o \
	$(BUILD_DIR)/vec3f.o

### Main ###
all: $(BUILD_DIR)/$(EXE)

### Link ###
$(BUILD_DIR)/$(EXE): $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $(BUILD_DIR)/$(EXE)

### Build ###
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/face.o: $(SRC_DIR)/face.c $(SRC_DIR)/face.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/model.o: $(SRC_DIR)/model.c $(SRC_DIR)/model.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/screen.o: $(SRC_DIR)/screen.c $(SRC_DIR)/screen.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/screen_draw.o: $(SRC_DIR)/screen_draw.c $(SRC_DIR)/screen_draw.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/util.o: $(SRC_DIR)/util.c $(SRC_DIR)/util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vec2i.o: $(SRC_DIR)/vec2i.c $(SRC_DIR)/vec2i.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vec3f.o: $(SRC_DIR)/vec3f.c $(SRC_DIR)/vec3f.h
	$(CC) $(CFLAGS) -c $< -o $@

### Clean ###
clean:
	$(RM) $(BUILD_DIR)/$(EXE)
	$(RM) $(OBJS)
