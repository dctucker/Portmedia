#include "common.h"

#include "adsr.h"
#include "piano.h"
#include "led.h"
#include "bcr.h"
#include "filter.h"
#include "scope.h"

static const GLuint WIDTH = 1024, HEIGHT = 768;
GLFWwindow* window = NULL;

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
	drawLoop();
}

int main(void)
{
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	window = glfwCreateWindow(WIDTH, HEIGHT, "GL Keys", NULL, NULL);
	glfwSetWindowAspectRatio(window, 4, 3);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit();
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
	glfwSetCharCallback(window, key_callback);
	//glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowRefreshCallback(window, window_refresh_callback);
	glfwSetDropCallback(window, drop_callback);

	while (!glfwWindowShouldClose(window))
	{
		glfwGetCursorPos(window, &mouseX, &mouseY);
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
