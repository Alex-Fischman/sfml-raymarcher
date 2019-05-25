BIN = raymarcher
CPP = *.cpp
GLSL = *.glsl
PKG = -lGL -lGLEW -lsfml-window -lsfml-system

all: $(BIN) run

run:
	./$(BIN)

$(BIN): $(CPP) $(GLSL)
	g++ $(CPP) $(PKG) -o $(BIN)

clean:
	rm $(BIN)
