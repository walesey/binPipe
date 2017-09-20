CC = gcc
CFLAGS = -lGL -lGLU -lglut
CFLAGS_MAC = -framework OpenGL -framework GLUT

BUILD_DIR = build

OUTPUT_3D = binPipe3D
OUTPUT_2D = binPipe2D
OUTPUT_BENCH = bench

SOURCE_FILES_3D = main3D.c renderers/renderer3D.c renderers/textfile.c renderers/math.c binary/binary.c
SOURCE_FILES_2D = main2D.c renderers/renderer2D.c renderers/textfile.c renderers/math.c binary/binary.c
SOURCE_FILES_BENCH = mainBench.c renderers/renderer2D.c renderers/textfile.c renderers/math.c

OUTPUT = $(OUTPUT_3D)
SOURCE_FILES = $(SOURCE_FILES_3D)

run: clean compile 
	./$(BUILD_DIR)/$(OUTPUT)

runMac: clean compileMac 
	./$(BUILD_DIR)/$(OUTPUT)

compile:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(OUTPUT) $(SOURCE_FILES) $(CFLAGS)

compileMac:
	mkdir -p $(BUILD_DIR)
	$(CC) -o $(BUILD_DIR)/$(OUTPUT) $(SOURCE_FILES) $(CFLAGS_MAC)

clean:
	rm -fr $(BUILD_DIR)

all: clean build
