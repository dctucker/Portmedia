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
	layout (location = 0) in float velocity;
	out float vertColor;
	out int note;
	void main() {
		vertColor = velocity;
		note = int(gl_VertexID) + 21;
	}
);
GLSL(geometryShaderSource,

	layout (points) in;
	layout (triangle_strip, max_vertices=256) out;

	in float vertColor[];
	in int   note[];
	out vec3 fragColor;

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

	void main() {

		float c = vertColor[0];
		int key = note[0] % 12;
		int oct = note[0] / 12;
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
GLfloat velocities[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.9f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.0f,
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(velocities), velocities, GL_STATIC_DRAW);
	/* Position attribute */
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, sizeof(velocities) / sizeof(GLfloat) );
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
