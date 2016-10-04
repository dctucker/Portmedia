#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define INFOLOG_LEN 512

#define GLSL(var, shader) static const GLchar* var = "#version 330 core\n" #shader

static const GLuint WIDTH = 800, HEIGHT = 600;
/* vertex data is passed as input to this shader
 * ourColor is passed as input to the to the fragment shader.
 */
GLSL(vertexShaderSource,
	layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;
	out vec3 vertColor;
	void main() {
		gl_Position = vec4(position, 1.0f);
		vertColor = color;
	}
);
GLSL(geometryShaderSource,
	layout (points) in;
	layout (triangle_strip, max_vertices=96) out;

	in vec3 vertColor[];
	out vec3 fragColor;

	void main() {
		fragColor = vertColor[0];

		gl_Position = gl_in[0].gl_Position + vec4(-0.04, 0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03, 0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03, 0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00, 0.08, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00, 0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01, 0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01, 0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03, 0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03, 0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04, 0.08, 0, 0); EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position + vec4(-0.04,-0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03,-0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03,-0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01,-0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01,-0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00,-0.08, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00,-0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01,-0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01,-0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03,-0.09, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03,-0.07, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04,-0.08, 0, 0); EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position + vec4(-0.04, 0.00, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03, 0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03,-0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01,-0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00, 0.00, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00, 0.00, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01, 0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01,-0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03, 0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03,-0.01, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04, 0.00, 0, 0); EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position + vec4(-0.04, 0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.05, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.05, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.04, 0.00, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00, 0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00, 0.00, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04, 0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.05, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.05, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04, 0.00, 0, 0); EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position + vec4(-0.04,-0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.05,-0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03,-0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.05,-0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03,-0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.04,-0.00, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00,-0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01,-0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01,-0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01,-0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.01,-0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.00,-0.00, 0, 0); EmitVertex();
		EndPrimitive();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04,-0.08, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.05,-0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03,-0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.05,-0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.03,-0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( 0.04,-0.00, 0, 0); EmitVertex();
		EndPrimitive();

		gl_Position = gl_in[0].gl_Position + vec4(-0.04, 0.05, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.04, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.03, 0.06, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.02, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.02, 0.03, 0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4(-0.01, 0.02, 0, 0); EmitVertex();
		EndPrimitive();
	}
);
GLSL(fragmentShaderSource,
	in vec3 fragColor;
	out vec4 outColor;
	void main() {
		outColor = vec4( fragColor, 1.0 );
	}
);
GLfloat vertices[] = {
	/*   Positions            Colors */
	 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
	 0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
};

#define SETUP_SHADER(type, shader, shadersource) GLint shader = glCreateShader(type); { \
	GLint success; \
	GLchar infoLog[INFOLOG_LEN]; \
	glShaderSource(shader, 1, &shadersource, NULL); \
	glCompileShader(shader); \
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success); \
	if (!success) { \
		glGetShaderInfoLog(shader, INFOLOG_LEN, NULL, infoLog); \
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog); \
	} \
}

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, __FILE__, NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	/* Build and compile shader program. */
	SETUP_SHADER( GL_VERTEX_SHADER, vertexShader, vertexShaderSource );
	SETUP_SHADER( GL_GEOMETRY_SHADER, geometryShader, geometryShaderSource );
	SETUP_SHADER( GL_FRAGMENT_SHADER, fragmentShader, fragmentShaderSource );

	/* Link shaders */
	GLint shaderProgram = glCreateProgram(); {
		GLint success;
		GLchar infoLog[INFOLOG_LEN];
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, geometryShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, INFOLOG_LEN, NULL, infoLog);
			printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
		}
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/* Position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* Color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, 3);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
