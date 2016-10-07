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
	layout (location = 0) in float in_f0;
	layout (location = 1) in float in_f1;
	out float f0;
	out float f1;
	out int index;
	void main() {
		f0 = in_f0;
		f1 = in_f1;
		index = gl_VertexID;
	}
);
GLSL(geometryShaderSource,

	layout (points) in;
	layout (triangle_strip, max_vertices=6) out;

	in float f0[];
	in float f1[];
	in int index[];
	out vec4 fragColor;

	void main(){
		float alpha = 0.4;
		int mindb = -36;
		int maxdb = 24;
		int x = 0; //-inst;
		int y = 0; //inst * 0.2f;
		int z = 0; //inst / 8.0f;
		vec3 scaleVector = vec3( 1.0/128.0, 1.0/100.0, 1 );
		
		fragColor = vec4(0.0, 1.0 ,0.0, alpha );
		
		//glNormal3f( 0.0, 0.0, 1.0 );

		int i = index[0];

		//glTranslatef( -0.02, -0.35, 0.0 );

		float f_0 = clamp( f0[0], mindb, maxdb );
		float f_1 = clamp( f1[0], mindb, maxdb );

		gl_Position = vec4( scaleVector * vec3(   i + x , f_0   + y , z ), 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec3( i+1 + x , f_1   + y , z ), 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec3(   i + x , mindb + y , z ), 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec3( i+1 + x , mindb + y , z ), 1.0); EmitVertex();
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
GLfloat filter_db[] = {
	-36,
	-30,
	-24,
	-20,
	-18,
	-15,
	-12,
	-9,
	-3,
	 2,
	 6,
	 7,
	 6,
	 2,
	 0,
	-5,
	-10,
	-11,
	-16,
	-20,
	-30,
	-44
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
	glBufferData(GL_ARRAY_BUFFER, sizeof(filter_db), filter_db, GL_STATIC_DRAW);
	/* filter_db[i] */
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* filter_db[i+1] */
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)(1 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, (sizeof(filter_db) - 1) / sizeof(GLfloat) );
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
