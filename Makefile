CC = gcc
CFLAGS = -lGL -lGLU -lglut -lpthread -lm -std=gnu99
CFLAGS_MAC = -framework OpenGL -framework GLUT

BUILD_DIR = build
OUTPUT = binPipe
SOURCE_FILES = main.c renderers/common.c renderers/renderer2D.c renderers/renderer3D.c renderers/textfile.c renderers/math.c binary/binary.c

run: clean compile
	./$(BUILD_DIR)/$(OUTPUT)

runMac: clean compileMac
	./$(BUILD_DIR)/$(OUTPUT)

run2D: clean compile
	./$(BUILD_DIR)/$(OUTPUT) 2D

runMac2D: clean compileMac 
	./$(BUILD_DIR)/$(OUTPUT) 2D

compile:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(OUTPUT) $(SOURCE_FILES) $(CFLAGS)

compileMac:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(OUTPUT) $(SOURCE_FILES) $(CFLAGS_MAC)

clean:
	rm -fr $(BUILD_DIR)

all: clean build
