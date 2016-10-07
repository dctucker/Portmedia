INCLUDES=-I/usr/local/Cellar/glfw3/3.2.1/include -I/usr/local/Cellar/glew/2.0.0/include
LIBDIRS=-L/usr/local/Cellar/glfw3/3.2.1/lib -L/usr/local/Cellar/glew/2.0.0/lib

EXECUTABLE=gl

default: gl led bcr piano

gl: gl.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

led: led.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

piano: piano.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

scope: scope.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

bcr: bcr.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

filter: filter.c
	gcc ${INCLUDES} ${LIBDIRS} -framework OpenGL -lglew -lglfw3 -o $@ $@.c

clean:
	rm -f gl led
