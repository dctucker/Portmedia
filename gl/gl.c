#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define INFOLOG_LEN 512

#define GLSL(var, shader) static const GLchar* var = ("#version 330 core\n" #shader)
#define HEREDOC(var) #var

static const GLuint WIDTH = 800, HEIGHT = 600;
/* vertex data is passed as input to this shader
 * ourColor is passed as input to the to the fragment shader.
 */
GLSL(vertexShaderSource,
	layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;
	layout (location = 2) in float fChar;
	out vec3 vertColor;
	out int character;
	void main() {
		gl_Position = vec4(position, 1.0f);
		vertColor = color;
		character = int(fChar);
	}
);
GLSL(geometryShaderSource,

	float kloc[13] = float[]( 0, 1.00, 2.00, 2.50, 3.50, 4.50, 5.50, 6.50, 7.00, 8.00, 8.50, 9.50, 10.50 );
	float wloc[13] = float[]( 0, 1.45, 1.55, 2.95, 3.05, 4.50, 5.95, 6.05, 7.45, 7.55, 8.95, 9.05, 10.45 );
	float sep = 0.05;
	float phei = 0.12;
	float alpha = 1.0;
	float aleft = 0.0;
	float arigh = 0.07;
	float atop = 0.0;
	float abot = 0.9;
	float ktop = 0.9;
	float kmid = 0.7;
	float kbot = 0.5;
	float kleft = arigh;
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
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (kloc[q] + sep), ktop, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (kloc[p] - sep), ktop, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (kloc[q] + sep), kmid, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (kloc[p] - sep), kmid, kz0, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( 0, 0, -1 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), kmid, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), kmid, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), kbot, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), kbot, kz0, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( 0, 1, 0 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), kbot, kz1, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), kbot, kz1, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), kbot, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), kbot, kz0, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( -1, 0, 0 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), ktop, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), ktop, kz1, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), kbot, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[p] - sep), kbot, kz1, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( 1, 0, 0 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), ktop, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), ktop, kz1, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), kbot, kz0, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * (wloc[q] + sep), kbot, kz1, 0); EmitVertex();
EndPrimitive();
	}

	void blackkey(in int q, in int p) {
		//gl_Position = gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * vec4(0.004, 0.008, 1.0, 1.0);
		//EmitVertex();
		//EndPrimitive();

		//glNormal3f( 0, 0, -1 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], ktop, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], ktop, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], kmid, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], kmid, nz, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( -1, 0, 0 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], ktop, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], ktop, wz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], kmid, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], kmid, wz, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( 1, 0, 0 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], ktop, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], ktop, wz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], kmid, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], kmid, wz, 0); EmitVertex();
EndPrimitive();
		//glNormal3f( 0, 1, 0 );
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], kmid, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], kmid, nz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[q], kmid, wz, 0); EmitVertex();
		gl_Position = gl_in[0].gl_Position + vec4( n + nw * kloc[p], kmid, wz, 0); EmitVertex();
EndPrimitive();
	}

	layout (points) in;
	layout (triangle_strip, max_vertices=256) out;

	in vec3 vertColor[];
	in int character[];
	out vec3 fragColor;

	void main() {

		int key = character[0] % 12;
		int oct = character[0] / 12;
		n = kleft;
		n += oct * (nw * 10.5);

		bool white = (key % 2) == (key > 4 ? 1 : 0);
		if( white ){
			fragColor = vec3( 1.1, 1.0, 1.0);
			whitekey(key, key + 1);
		} else {
			fragColor = vec3( 0.3, 0.3, 0.3);
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
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 0,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 2,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 3,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 4,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 5,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 6,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 7,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 8,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 9,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 10,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 11,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 12,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 13,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 14,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 15,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 16,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 17,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 18,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 19,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 20,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 21,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 22,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 23,
	-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 24
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* Color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	/* Character attribute */
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, sizeof(vertices) / sizeof(GLfloat) / 7);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
