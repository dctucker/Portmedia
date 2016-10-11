INCLUDES=-I/usr/local/Cellar/glfw3/3.2.1/include -I/usr/local/Cellar/glew/2.0.0/include
LIBDIRS=-L/usr/local/Cellar/glfw3/3.2.1/lib -L/usr/local/Cellar/glew/2.0.0/lib

EXECUTABLE=gl

default: gl

gl: gl.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

clean:
	rm -f gl
