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
	layout (location = 0) in vec2 in_min_max_0;
	layout (location = 1) in vec2 in_min_max_1;
	uniform mat4 MVP;
	out vec2 min_max_0;
	out vec2 min_max_1;
	out int index;
	void main() {
		min_max_0 = in_min_max_0;
		min_max_1 = in_min_max_1;
		index = gl_VertexID;
	}
);
GLSL(geometryShaderSource,

	layout (points) in;
	layout (triangle_strip, max_vertices=6) out;

	uniform mat4 MVP;
	in vec2 min_max_0[];
	in vec2 min_max_1[];
	in int index[];
	out vec4 fragColor;

	void main(){
		float alpha = 0.75;
		float height = 0.5;
		float width = 349.0;
		float sleft = 0.0;
		float srigh = sleft + 1.0;
		//float stop = 1.0;
		//float sbot = stop - height;
		float center_y = 0.5;
		float dx = (1.0 / width) * (srigh - sleft);
		
		//glTranslatef( 0, 0, 0.9 );
		//glBegin(GL_POLYGON);
		//glNormal3f( 0, 0, -1 );
		/*
		glColor4f( 0.1, 0.2, 0.1, alpha );
		glVertex2f( sleft, stop );
		glVertex2f( sleft, sbot );
		glVertex2f( srigh, sbot );
		glVertex2f( srigh, stop );
		*/
		//glEnd();
		
		//glNormal3f( 0.0, 0.0, 1.0 );

		int i = index[0];
		float scope_minv_0 = min_max_0[0].x;
		float scope_minv_1 = min_max_1[0].x;
		float scope_maxv_0 = min_max_0[0].y;
		float scope_maxv_1 = min_max_1[0].y;

		fragColor = vec4(0.6, 1.0 ,0.6, alpha );
		gl_Position = MVP * vec4(     i * dx + sleft, center_y - height * scope_maxv_0, 1.0, 1.0 ); EmitVertex();
		gl_Position = MVP * vec4( (i+1) * dx + sleft, center_y - height * scope_maxv_1, 1.0, 1.0 ); EmitVertex();
		fragColor = vec4(0.6, 1.0 ,0.6, 0.1 );
		gl_Position = MVP * vec4(     i * dx + sleft, center_y, 1.0, 1.0 ); EmitVertex();
		gl_Position = MVP * vec4( (i+1) * dx + sleft, center_y, 1.0, 1.0 ); EmitVertex();
		fragColor = vec4(0.6, 1.0 ,0.6, alpha );
		gl_Position = MVP * vec4(     i * dx + sleft, center_y - height * scope_minv_0, 1.0, 1.0 ); EmitVertex();
		gl_Position = MVP * vec4( (i+1) * dx + sleft, center_y - height * scope_minv_1, 1.0, 1.0 ); EmitVertex();
		EndPrimitive();
	}
);
GLSL(fragmentShaderSource,
	in vec4 fragColor;
	out vec4 outColor;
	void main() {
		outColor = fragColor;
	}
);
GLfloat min_max[] = {
	 0.0, 0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 1.0,  1.0,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-1.0, -1.0,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
	-0.4, -0.4,
	-0.3, -0.3,
	-0.2, -0.2,
	-0.1, -0.1,
	 0.0,  0.0,
	-0.1, -0.9,
	-0.2, -0.8,
	-0.3, -0.7,
	-0.4, -0.6,
	-0.5, -0.5,
	-0.6, -0.4,
	-0.7, -0.3,
	-0.8, -0.2,
	-0.9, -0.1,
	 0.0,  0.0,
	 0.1,  0.1,
	 0.2,  0.2,
	 0.3,  0.3,
	 0.4,  0.4,
	 0.5,  0.5,
	 0.6,  0.6,
	 0.7,  0.7,
	 0.8,  0.8,
	 0.9,  0.9,
	 0.8,  0.8,
	 0.7,  0.7,
	 0.6,  0.6,
	 0.5,  0.5,
	 0.4,  0.4,
	 0.3,  0.3,
	 0.2,  0.2,
	 0.1,  0.1,
	 0.0,  0.0,
	-0.1, -0.1,
	-0.2, -0.2,
	-0.3, -0.3,
	-0.4, -0.4,
	-0.5, -0.5,
	-0.6, -0.6,
	-0.7, -0.7,
	-0.8, -0.8,
	-0.9, -0.9,
	-0.8, -0.8,
	-0.7, -0.7,
	-0.6, -0.6,
	-0.5, -0.5,
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
		1.8,0,0,-0.9,
		0,0.75,0,0,
		0,0,1,0,
		0,0,0,1
	};
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
	GLint timeUniform = glGetUniformLocation(shaderProgram,"time");
	GLint projectionUniform = glGetUniformLocation(shaderProgram,"MVP");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(min_max), min_max, GL_STATIC_DRAW);
	/* min_max[i] */
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* min_max[i+1] */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glUniformMatrix4fv(projectionUniform, 1, GL_TRUE, &projectionMatrix[0]);
		glUniform1f(timeUniform, (float)glfwGetTime());
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, (sizeof(min_max) / sizeof(GLfloat))-1 );
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
