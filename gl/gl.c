#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define INFOLOG_LEN 512

#define GLSL(var, shader) static const GLchar* var = ("#version 330 core\n" #shader)
#define HEREDOC(var) #var

static const GLuint WIDTH = 1024, HEIGHT = 768;
/* vertex data is passed as input to this shader
 * ourColor is passed as input to the to the fragment shader.
 */
GLSL(vertexShaderSource,
	layout (location = 0) in float color;
	layout (location = 1) in float fChar;
	out float vertColor;
	out int character;
	void main() {
		vertColor = color;
		character = int(fChar);
	}
);
GLSL(geometryShaderSource,

	float kloc[13] = float[]( 0, 1.00, 2.00, 2.50, 3.50, 4.50, 5.50, 6.50, 7.00, 8.00, 8.50, 9.50, 10.50 );
	float wloc[13] = float[]( 0, 1.45, 1.55, 2.95, 3.05, 4.50, 5.95, 6.05, 7.45, 7.55, 8.95, 9.05, 10.45 );
	float sep = 0.08;
	float phei = 0.12;
	float alpha = 1.0;
	float aleft = 0.0;
	float arigh = 0.07;
	float atop = 0.0;
	float abot = 0.9;
	float ktop = -0.8;
	float kmid = -0.9;
	float kbot = -1.0;
	float kleft = arigh-0.9;
	float krigh = 1.0;
	float n;
	float n1;
	float az  =  0.1;
	float bz  =  0.0;
	float wz  = -0.1;
	float kz0 = -0.1;
	float kz1 =  0.0;
	float nz  = -0.2;
	float cz  = -0.3;
	float spz = -0.3;
	float nw = (krigh-kleft) / 78.0;
	
	void whitekey(in int q, in int p);
	void whitekey(in int q, in int p) {
		//gl_Position = gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * vec4(0.004, 0.008, 1.0, 1.0);
		//EmitVertex();
		//EndPrimitive();

		//glNormal3f( 0, 0, -1 )
		gl_Position = vec4( n + nw * (kloc[q] + sep), ktop, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (kloc[p] - sep), ktop, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (kloc[q] + sep), kmid, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (kloc[p] - sep), kmid, kz0, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( 0, 0, -1 );
		gl_Position = vec4( n + nw * (wloc[q] + sep), kmid, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[p] - sep), kmid, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[q] + sep), kbot, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[p] - sep), kbot, kz0, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( 0, 1, 0 );
		gl_Position = vec4( n + nw * (wloc[p] - sep), kbot, kz1, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[q] + sep), kbot, kz1, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[p] - sep), kbot, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[q] + sep), kbot, kz0, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( -1, 0, 0 );
		gl_Position = vec4( n + nw * (wloc[p] - sep), ktop, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[p] - sep), ktop, kz1, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[p] - sep), kbot, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[p] - sep), kbot, kz1, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( 1, 0, 0 );
		gl_Position = vec4( n + nw * (wloc[q] + sep), ktop, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[q] + sep), ktop, kz1, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[q] + sep), kbot, kz0, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * (wloc[q] + sep), kbot, kz1, 1.0); EmitVertex();
		EndPrimitive();
	}

	void blackkey(in int q, in int p) {
		//gl_Position = gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * vec4(0.004, 0.008, 1.0, 1.0);
		//EmitVertex();
		//EndPrimitive();

		//glNormal3f( 0, 0, -1 );
		gl_Position = vec4( n + nw * kloc[q], ktop, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], ktop, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[q], kmid, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], kmid, nz, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( -1, 0, 0 );
		gl_Position = vec4( n + nw * kloc[q], ktop, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[q], ktop, wz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[q], kmid, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[q], kmid, wz, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( 1, 0, 0 );
		gl_Position = vec4( n + nw * kloc[p], ktop, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], ktop, wz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], kmid, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], kmid, wz, 1.0); EmitVertex();
		EndPrimitive();
		//glNormal3f( 0, 1, 0 );
		gl_Position = vec4( n + nw * kloc[q], kmid, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], kmid, nz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[q], kmid, wz, 1.0); EmitVertex();
		gl_Position = vec4( n + nw * kloc[p], kmid, wz, 1.0); EmitVertex();
		EndPrimitive();
	}

	layout (points) in;
	layout (triangle_strip, max_vertices=256) out;

	in float vertColor[];
	in int   character[];
	out vec3 fragColor;

	void main() {

		float c = vertColor[0];
		int key = character[0] % 12;
		int oct = character[0] / 12;
		n = kleft;
		n += (oct-2) * (nw * 10.5);

		bool white = (key % 2) == (key > 4 ? 1 : 0);
		if( white ){
			fragColor = vec3( 1.0, 1.0 - c, 1.0 - c);
			whitekey(key, key + 1);
		} else {
			fragColor = vec3( 0.3 + c, 0.3 * (1.0 - c), 0.3 * (1.0 - c) );
			blackkey(key, key + 1);
		}
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
	0.0f, 21,
	0.0f, 22,
	0.0f, 23,
	0.0f, 24,
	0.0f, 25,
	0.0f, 26,
	0.0f, 27,
	0.0f, 28,
	0.0f, 29,
	0.0f, 30,
	1.0f, 31,
	0.0f, 32,
	0.0f, 33,
	0.0f, 34,
	0.0f, 35,
	0.0f, 36,
	0.0f, 37,
	1.0f, 38,
	0.0f, 39,
	0.0f, 40,
	0.0f, 41,
	0.0f, 42,
	0.9f, 43,
	0.0f, 44,
	0.0f, 45,
	0.0f, 46,
	0.0f, 47,
	0.0f, 48,
	0.0f, 49,
	1.0f, 50,
	0.0f, 51,
	0.0f, 52,
	0.0f, 53,
	0.0f, 54,
	0.6f, 55,
	0.0f, 56,
	0.0f, 57,
	0.0f, 58,
	1.0f, 59,
	0.0f, 60,
	0.0f, 61,
	0.0f, 62,
	0.0f, 63,
	0.0f, 64,
	0.0f, 65,
	0.0f, 66,
	0.0f, 67,
	0.0f, 68,
	0.0f, 69,
	0.0f, 70,
	0.0f, 71,
	0.0f, 72,
	0.0f, 73,
	0.0f, 74,
	0.0f, 75,
	0.0f, 76,
	0.0f, 77,
	0.0f, 78,
	0.0f, 79,
	0.0f, 80,
	0.0f, 81,
	0.0f, 82,
	0.0f, 83,
	0.0f, 84,
	0.0f, 85,
	0.0f, 86,
	0.0f, 87,
	0.0f, 88,
	0.0f, 89,
	0.0f, 90,
	0.0f, 91,
	0.0f, 92,
	0.0f, 93,
	0.0f, 94,
	0.0f, 95,
	0.0f, 96,
	0.0f, 97,
	0.0f, 98,
	0.0f, 99,
	0.0f, 100,
	0.0f, 101,
	0.0f, 102,
	0.0f, 103,
	0.0f, 104,
	0.0f, 105,
	0.0f, 106,
	0.0f, 107,
	0.0f, 108
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GL Keys", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
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
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* Color attribute */
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(1 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, sizeof(vertices) / sizeof(GLfloat) / 2);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
