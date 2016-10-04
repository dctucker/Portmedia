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
	int led16_vertex_data[] = int[](
		// top left
		-9,  9,
		-8, 10,
		-8,  8,
		-1, 10,
		-1,  8,
		 0,  9,
		// top right
		 0,  9,
		 1, 10,
		 1,  8,
		 8, 10,
		 8,  8,
		 9,  9,
		// upper right
		 8,  1,
		 8,  8,
		 9,  0,
		 9,  9,
		10,  1,
		10,  8,
		// lower right
		 8, -1,
		 8, -8,
		 9,  0,
		 9, -9,
		10, -1,
		10, -8,
		// bottom right
		 0,  -9,
		 1, -10,
		 1,  -8,
		 8, -10,
		 8,  -8,
		 9,  -9,
		// bottom left
		-9,  -9,
		-8, -10,
		-8,  -8,
		-1, -10,
		-1,  -8,
		 0,  -9,
		// lower left
		-8,  -1,
		-8,  -8,
		-9,   0,
		-9,  -9,
		-10, -1,
		-10, -8,
		// upper left
		-10, 8,
		-10, 1,
		-9,  9,
		-9,  0,
		-8,  8,
		-8,  1,
		// diagonal upper left
		 0, 0,
		-1, 1,
		-1, 2,
		-8, 7,
		-7, 8,
		-8, 8,
		// top center
		-1, 2,
		-1, 8,
		 0, 0,
		 0, 9,
		 1, 2,
		 1, 8,
		// diagonal upper right
		 0, 0,
		 1, 2,
		 1, 1,
		 7, 8,
		 8, 7,
		 8, 8,
		// right middle
		 0,  0,
		 2, -1,
		 2,  1,
		 8, -1,
		 8,  1,
		 9,  0,
		// diagonal lower right
		 0,  0,
		 1, -1,
		 1, -2,
		 8, -7,
		 7, -8,
		 8, -8,
		// bottom center
		-1, -2,
		-1, -8,
		 0,  0,
		 0, -9,
		 1, -2,
		 1, -8,
		// diagonal lower left
		 0,  0,
		-1, -2,
		-1, -1,
		-7, -8,
		-8, -7,
		-8, -8,
		// left middle
		 0,  0,
		-2,  1,
		-2, -1,
		-8,  1,
		-8, -1,
		-9,  0
	);
	int led16_ee[] = int[](
		0x4000,
		0x4001,
		0x4002,
		0x4003,
		0x4004,
		0x4005,
		0x4006,
		0x4007,
		0x4008,
		0x4009,
		0x400a,
		0x400b,
		0x400c,
		0x400d,
		0x400e,
		0x400f,
		0x4010,
		0x4011,
		0x4012,
		0x4013,
		0x4014,
		0x4015,
		0x4016,
		0x4017,
		0x4018,
		0x4019,
		0x401a,
		0x401b,
		0x401c,
		0x401d,
		0x401e,
		0x401f,
		        //   0 1 2 3  4 5 6 7  8 9 a b  c d e f    //
		0x0000, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }, // 
		0x8541, // { 1,0,0,0, 0,1,0,1, 0,1,0,0, 0,0,0,1 }, // 
		0x2100, // { 0,0,1,0, 0,0,0,1, 0,0,0,0, 0,0,0,0 }, //"
		0x3c55, // { 0,0,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,1 }, //#
		0xdd55, // { 1,1,0,1, 1,1,0,1, 0,1,0,1, 0,1,0,1 }, //$
		0x9977, // { 1,0,0,1, 1,0,0,1, 0,1,1,1, 0,1,1,1 }, //%
		0x875d, // { 1,0,0,0, 0,1,1,1, 0,1,0,1, 1,1,0,1 }, //&
		0x0040, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0 }, //'
		0x0028, // { 0,0,0,0, 0,0,0,0, 0,0,1,0, 1,0,0,0 }, //(
		0x0082, // { 0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,1,0 }, //)
		0x00ff, // { 0,0,0,0, 0,0,0,0, 1,1,1,1, 1,1,1,1 }, //*
		0x0055, // { 0,0,0,0, 0,0,0,0, 0,1,0,1, 0,1,0,1 }, //+
		0x0003, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1 }, //,
		0x0011, // { 0,0,0,0, 0,0,0,0, 0,0,0,1, 0,0,0,1 }, //-
		0x0400, // { 0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0 }, //.
		0x0022, // { 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,0,1,0 }, ///
		0xff22, // { 1,1,1,1, 1,1,1,1, 0,0,1,0, 0,0,1,0 }, //0
		0x8c44, // { 1,0,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //1
		0xee11, // { 1,1,1,0, 1,1,1,0, 0,0,0,1, 0,0,0,1 }, //2
		0xfc11, // { 1,1,1,1, 1,1,0,0, 0,0,0,1, 0,0,0,1 }, //3
		0x3111, // { 0,0,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 }, //4
		0xdc90, // { 1,1,0,1, 1,1,0,0, 1,0,0,1, 0,0,0,0 }, //5
		0xdf11, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //6
		0xc023, // { 1,1,0,0, 0,0,0,0, 0,0,1,0, 0,0,1,1 }, //7
		0xff11, // { 1,1,1,1, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //8
		0xfd11, // { 1,1,1,1, 1,1,0,1, 0,0,0,1, 0,0,0,1 }, //9
		0x8400, // { 1,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0 }, //:
		0x8003, // { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1 }, //;
		0x0c22, // { 0,0,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //<
		0x0c11, // { 0,0,0,0, 1,1,0,0, 0,0,0,1, 0,0,0,1 }, //=
		0x0c88, // { 0,0,0,0, 1,1,0,0, 1,0,0,0, 1,0,0,0 }, //>
		0xe014, // { 1,1,1,0, 0,0,0,0, 0,0,0,1, 0,1,0,0 }, //?
		0xfe05, // { 1,1,1,1, 1,1,1,0, 0,0,0,0, 0,1,0,1 }, //@
		0x3032, // { 0,0,1,1, 0,0,0,0, 0,0,1,1, 0,0,1,0 }, //A
		0xfc54, // { 1,1,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,0 }, //B
		0xcf00, // { 1,1,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //C
		0xfc44, // { 1,1,1,1, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //D
		0xcf11, // { 1,1,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //E
		0xc311, // { 1,1,0,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //F
		0xdf10, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,0 }, //G
		0x3311, // { 0,0,1,1, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //H
		0xcc44, // { 1,1,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //I
		0xc644, // { 1,1,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,0 }, //J
		0x0329, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 1,0,0,1 }, //K
		0x0f00, // { 0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //L
		0x33a4, // { 0,0,1,1, 0,0,1,1, 1,0,1,0, 0,1,0,0 }, //M
		0x3388, // { 0,0,1,1, 0,0,1,1, 1,0,0,0, 1,0,0,0 }, //N
		0xff00, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //O
		0xe311, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //P
		0xff08, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 1,0,0,0 }, //Q
		0xe319, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 1,0,0,1 }, //R
		0xee88, // { 1,1,1,0, 1,1,1,0, 1,0,0,0, 1,0,0,0 }, //S
		0xc044, // { 1,1,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //T
		0x3f00, // { 0,0,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //U
		0x0322, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 0,0,1,0 }, //V
		0x334a, // { 0,0,1,1, 0,0,1,1, 0,1,0,0, 1,0,1,0 }, //W
		0x00aa, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 1,0,1,0 }, //X
		0x00a4, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 0,1,0,0 }, //Y
		0xcc22, // { 1,1,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //Z
		0x4844, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,0 }, //[
		0x0088, // { 0,0,0,0, 0,0,0,0, 1,0,0,0, 1,0,0,0 }, //\/
		0x8444, // { 1,0,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0 }, //]
		0x2020, // { 0,0,1,0, 0,0,0,0, 0,0,1,0, 0,0,0,0 }, //^
		0x0c00, // { 0,0,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0 }, //_
		0x0080, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0 }, //`
		//
		0x3032, // { 0,0,1,1, 0,0,0,0, 0,0,1,1, 0,0,1,0 }, //A
		0xfc54, // { 1,1,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,0 }, //B
		0xcf00, // { 1,1,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //C
		0xfc44, // { 1,1,1,1, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //D
		0xcf11, // { 1,1,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //E
		0xc311, // { 1,1,0,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //F
		0xdf10, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,0 }, //G
		0x3311, // { 0,0,1,1, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //H
		0xcc44, // { 1,1,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //I
		0xc644, // { 1,1,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,0 }, //J
		0x0329, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 1,0,0,1 }, //K
		0x0f00, // { 0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //L
		0x33a4, // { 0,0,1,1, 0,0,1,1, 1,0,1,0, 0,1,0,0 }, //M
		0x3388, // { 0,0,1,1, 0,0,1,1, 1,0,0,0, 1,0,0,0 }, //N
		0xff00, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //O
		0xe311, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //P
		0xff08, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 1,0,0,0 }, //Q
		0xe319, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 1,0,0,1 }, //R
		0xee88, // { 1,1,1,0, 1,1,1,0, 1,0,0,0, 1,0,0,0 }, //S
		0xc044, // { 1,1,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //T
		0x3f00, // { 0,0,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //U
		0x0322, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 0,0,1,0 }, //V
		0x334a, // { 0,0,1,1, 0,0,1,1, 0,1,0,0, 1,0,1,0 }, //W
		0x00aa, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 1,0,1,0 }, //X
		0x00a4, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 0,1,0,0 }, //Y
		0xcc22, // { 1,1,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //Z
		// a-z
		0x4845, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,1 }, //{
		0x0044, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //|
		0x8454, // { 1,0,0,0, 0,1,0,0, 0,1,0,1, 0,1,0,0 }, //}
		0xa150, // { 1,0,1,0, 0,0,0,1, 0,1,0,1, 0,0,0,0 }, //~
		0x0c0a  // { 0,0,0,0, 1,1,0,0, 0,0,0,0, 1,0,1,0 }, //DEL
	);
	layout (points) in;
	layout (triangle_strip, max_vertices=96) out;

	in vec3 vertColor[];
	in int character[];
	out vec3 fragColor;

	void main() {
		fragColor = vertColor[0];

		int ch = led16_ee[character[0]];
		int s = 0x8000;
		for(int seg=0; seg < 192; seg+=12){
			if( (s & ch) == 0 ){
				fragColor = vec3( 0.05, 0.05, 0.05 );
			} else {
				fragColor = vertColor[0];
			}
			for(int v=0; v < 12; v+=2){
				gl_Position = gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * vec4(0.004, 0.008, 1.0, 1.0);
				EmitVertex();
			}
			EndPrimitive();
			s >>= 1;
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
	-0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 'H',
	-0.4f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 'E',
	-0.3f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'L',
	-0.2f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'L',
	-0.1f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'O',
	 0.0f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, ' ',
	 0.1f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'W',
	 0.2f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'O',
	 0.3f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'R',
	 0.4f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'L',
	 0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 'D',
	 0.6f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, '!',
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
