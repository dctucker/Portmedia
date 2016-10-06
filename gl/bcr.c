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

	float alpha = 0.8f;
	float bleft = 0.0;
	float brigh = 0.15;
	float btop = 0.6;
	float bbot = 0.8;
	float bwid = brigh - bleft;
	float bhei = bbot - btop;

	float val = 0.4;

	//glPushMatrix();
	void drawBoard(){
		//glColor4f( 0.2, 0.2, 0.4, alpha );
		
		//glBegin(GL_QUADS);
			glNormal3f( 0, 0, -1 );
			glVertex2f( bleft, btop );
			glVertex2f( brigh, btop );
			glVertex2f( brigh, bbot );
			glVertex2f( bleft, bbot );
		//glEnd();

		// zoom into the BCR panel now
		//glTranslatef( bleft, btop, 0.0 );
		//glScalef( bwid, bhei, 1.0 );
	}

	void drawValue(in int row, in int col, in float val){
		//glPushMatrix();
		// zoom into the current element
		vec3 transVector = vec3( 0.01 + col * 0.125, 0.075 + row * 0.1, 0.0 );
		vec3 scaleVector = vec3( 0.10, 0.0625, 1.0 );

		//glBegin(GL_QUADS);
		//glNormal3f( 0, 0, -1 );

		outColor = vec4( 1.0, 0.0, 0.0, alpha );
		gl_Position = vec4( transVector + scaleVector * vec2( 0.0, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec2( val, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec2( 0.0, 1.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec2( val, 1.0, 1.0 ), 1.0); EmitVertex();
	
		outColor = vec4( 0.0, 0.0, 0.0, alpha );
		gl_Position = vec4( transVector + scaleVector * vec2( val, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec2( 1.0, 0.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec2( val, 1.0, 1.0 ), 1.0); EmitVertex();
		gl_Position = vec4( transVector + scaleVector * vec2( 1.0, 1.0, 1.0 ), 1.0); EmitVertex();
		EndPrimitive();
		//glEnd();
		//glPopMatrix();
	}

	void drawValues(){
		float val = 0.4;
		for(int row = 0; row < 9; row++) {
			for(int col = 0; col < 8; col++) {
				switch(row) {
					case 0:
					case 1:
					case 2:
					case 3:
						//val = bcr->getKnob(   (row + 4) * 10 + (col + 1) );
						break;
					case 4:
					case 5:
						//val = bcr->getButton( (row + 4) * 10 + (col + 1) );
						break;
					case 6:
					case 7:
					case 8:
						//val = bcr->getKnob(   (row - 5) * 10 + (col + 1) );
						break;
				}
			
				drawValue(row, col, val);
			}
		}
	}

	void main(){
		drawBoard();
		drawValues();
	}
);
GLfloat vertices[] = {
	/*   Positions            Colors */
	0.0f, 21,
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

