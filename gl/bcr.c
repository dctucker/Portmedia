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
	layout (location = 0) in float index;
	layout (location = 1) in float inVal;
	out int   row;
	out int   col;
	out float value;
	void main() {
		row = int(index) / 8;
		col = int(index) % 8;
		value = inVal;
	}
);
GLSL(geometryShaderSource,

	layout (points) in;
	layout (triangle_strip, max_vertices=8) out;

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

	/*
	//glPushMatrix();
	void drawBoard(){
		//glColor4f( 0.2, 0.2, 0.4, alpha );
		
		//glBegin(GL_QUADS);
		//glNormal3f( 0, 0, -1 );
		gl_Position = vec4( bleft, btop, 1.0, 1.0 ); EmitVertex();
		gl_Position = vec4( brigh, btop, 1.0, 1.0 ); EmitVertex();
		gl_Position = vec4( bleft, bbot, 1.0, 1.0 ); EmitVertex();
		gl_Position = vec4( brigh, bbot, 1.0, 1.0 ); EmitVertex();
		EndPrimitive();
		//glEnd();
	}
	*/

	void drawValue(in int r, in int c, in float val){

		// zoom into the BCR panel now
		//glTranslatef( bleft, btop, 0.0 );
		//glScalef( bwid, bhei, 1.0 );

		//glPushMatrix();
		// zoom into the current element
		vec3 transVector = vec3( 0.01 + c * 0.120, 0.075 - r * 0.1, 0.0 );
		vec3 scaleVector = vec3( 0.10, 0.0625, 1.0 );

		//glBegin(GL_QUADS);
		//glNormal3f( 0, 0, -1 );

		fragColor = vec4( 1.0, 0.0, 0.0, alpha );
		gl_Position = vec4( transVector + scaleVector * vec3( 0.0, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec3( 0.0, 1.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec3( val, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec3( val, 1.0, 1.0 ), 1.0); EmitVertex();
	
		fragColor = vec4( 0.1, 0.1, 0.1, alpha );
		gl_Position = vec4( transVector + scaleVector * vec3( val, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec3( val, 1.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec3( 1.0, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec3( 1.0, 1.0, 1.0 ), 1.0); EmitVertex();
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
	/* rowcol val */
	000, 0.11,
	001, 0.12,
	002, 0.13,
	003, 0.14,
	004, 0.15,
	005, 0.16,
	006, 0.17,
	007, 0.18,
	010, 0.21,
	011, 0.22,
	012, 0.23,
	013, 0.24,
	014, 0.25,
	015, 0.26,
	016, 0.27,
	017, 0.28,
	020, 0.31,
	021, 0.32,
	022, 0.33,
	023, 0.34,
	024, 0.35,
	025, 0.36,
	026, 0.37,
	027, 0.38,
	030, 0.41,
	031, 0.42,
	032, 0.43,
	033, 0.44,
	034, 0.45,
	035, 0.46,
	036, 0.47,
	037, 0.48,
	040, 0.41,
	041, 0.42,
	042, 0.43,
	043, 0.44,
	044, 0.45,
	045, 0.46,
	046, 0.47,
	047, 0.48,
	050, 0.51,
	051, 0.52,
	052, 0.53,
	053, 0.54,
	054, 0.55,
	055, 0.56,
	056, 0.57,
	057, 0.58,
	060, 0.61,
	061, 0.62,
	062, 0.63,
	063, 0.64,
	064, 0.65,
	065, 0.66,
	066, 0.67,
	067, 0.68,
	070, 0.71,
	071, 0.72,
	072, 0.73,
	073, 0.74,
	074, 0.75,
	075, 0.76,
	076, 0.77,
	077, 0.78,
	0100, 0.91,
	0101, 0.92,
	0102, 0.93,
	0103, 0.94,
	0104, 0.95,
	0105, 0.96,
	0106, 0.99,
	0107, 1.00
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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/* row,col attribute */
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

