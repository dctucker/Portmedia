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
	layout (location = 0) in float inVal;
	uniform mat4 MVP;
	out int   row;
	out int   col;
	out float value;
	void main() {
		row = int(gl_VertexID) / 8;
		col = int(gl_VertexID) % 8;
		value = inVal;
	}
);
GLSL(geometryShaderSource,

	layout (points) in;
	layout (triangle_strip, max_vertices=8) out;

	uniform mat4 MVP;
	in int   row[];
	in int   col[];
	in float value[];

	out vec4 fragColor;

	float alpha = 0.8f;
	float bleft = 0.0;
	float brigh = 0.15;
	float btop = 0.6;
	float bbot = 0.8;
	float bwid = brigh - bleft;
	float bhei = bbot - btop;

	void drawValue(in int r, in int c, in float val){

		// zoom into the BCR panel now
		//glTranslatef( bleft, btop, 0.0 );
		//glScalef( bwid, bhei, 1.0 );

		//glPushMatrix();
		// zoom into the current element
		vec3 transVector = vec3( 0.01 + c * 0.120, 0.9 - r * 0.1, 0.0 );
		vec3 scaleVector = vec3( 0.10, 0.0625, 1.0 );

		//glBegin(GL_QUADS);
		//glNormal3f( 0, 0, -1 );

		fragColor = vec4( 1.0, 0.0, 0.0, alpha );
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( 0.0, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( 0.0, 1.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 1.0, 1.0 ), 1.0); EmitVertex();
	
		fragColor = vec4( 0.1, 0.1, 0.1, alpha );
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 1.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( 1.0, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = MVP * vec4( transVector + scaleVector * vec3( 1.0, 1.0, 1.0 ), 1.0); EmitVertex();
		EndPrimitive();
		//glEnd();
		//glPopMatrix();
	}

	void main(){
		drawValue(row[0], col[0], value[0]);

		//drawBoard();
		//drawValues();
	}
);
GLSL(fragmentShaderSource,
	in vec4 fragColor;
	out vec4 outColor;

	void main() {
		outColor = fragColor;
	}
);
GLfloat vertices[] = {
	0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18,
	0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28,
	0.31, 0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38,
	0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48,
	0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48,
	0.51, 0.52, 0.53, 0.54, 0.55, 0.56, 0.57, 0.58,
	0.61, 0.62, 0.63, 0.64, 0.65, 0.66, 0.67, 0.68,
	0.71, 0.72, 0.73, 0.74, 0.75, 0.76, 0.77, 0.78,
	0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.99, 1.00
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
	GLfloat projectionMatrix[16] = {
		0.5,0,0,-0.95,
		0,0.5,0,-0.65,
		0,0,1,0,
		0,0,0,1
	};
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GL BCR", NULL, NULL);
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
	GLint projectionUniform = glGetUniformLocation(shaderProgram,"MVP");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/* row,col attribute */
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, &projectionMatrix[0]);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, sizeof(vertices) / sizeof(GLfloat) );
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}

