#include "common.h"

#include "adsr.h"
#include "piano.h"
#include "led.h"
#include "bcr.h"
#include "filter.h"
#include "scope.h"

static const GLuint WIDTH = 1024, HEIGHT = 768;
GLFWwindow* window = NULL;
GLFWcursor *ibeam;
GLFWcursor *crosshair;
double ledBot, ledTop;
int overLed = 0;

static inline void drawLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);
	global_time = (float)glfwGetTime();
	runProgram(&adsr);
	runProgram(&piano);
	runProgram(&led);
	runProgram(&bcr);
	runProgram(&filter);
	runProgram(&scope);
	glBindVertexArray(0);
	glfwSwapBuffers(window);
}

void window_refresh_callback(GLFWwindow* window)
{
	ledBot = (0.5 * HEIGHT) - (0.5 * led.verts.data[1] * led.MVP.data[5] * HEIGHT);
	ledTop = ledBot - (0.5 * HEIGHT * led.MVP.data[5]);

	drawLoop();
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse[0] = (2 * xpos / (double)WIDTH) - 1;
	mouse[1] = 1 - (2 * ypos / (double)HEIGHT);

	if( ypos >= ledTop && ypos <= ledBot )
	{
		if( overLed == 0 )
		{
			overLed = 1;
			glfwSetCursor(window, ibeam);
		}
	}
	else
	{
		if( overLed == 1 )
		{
			overLed = 0;
			glfwSetCursor(window, crosshair);
		}
	}
}

void setupGl()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupAdsr();
	setupPiano();
	setupLed();
	setupBcr();
	setupFilter();
	setupScope();

	glBindVertexArray(0);
}

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(WIDTH, HEIGHT, "GL Keys", NULL, NULL);
	crosshair = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
	ibeam = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	glfwSetWindowAspectRatio(window, 4, 3);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit();

	setupGl();

	glfwSetCharCallback(window, key_callback);
	//glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetDropCallback(window, drop_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);

	glfwSetCursor(window, crosshair);

	while (!glfwWindowShouldClose(window))
	{
		drawLoop();
		glfwPollEvents();
	}
	teardownProgram(&adsr);
	teardownProgram(&piano);
	teardownProgram(&led);
	teardownProgram(&bcr);
	teardownProgram(&filter);
	teardownProgram(&scope);
	glfwTerminate();
	return EXIT_SUCCESS;
}
