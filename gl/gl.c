#include "common.h"

#include "adsr.h"
#include "piano.h"
#include "led.h"
#include "bcr.h"
#include "filter.h"
#include "scope.h"

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GL Keys", NULL, NULL);
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

	while (!glfwWindowShouldClose(window))
	{
		global_time = (float)glfwGetTime();
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		runProgram(&adsr);
		runProgram(&piano);
		runProgram(&led);
		runProgram(&bcr);
		runProgram(&filter);
		runProgram(&scope);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &(adsr.verts.vao));
	glDeleteBuffers(1, &(adsr.verts.vbo));
	glfwTerminate();
	return EXIT_SUCCESS;
}
