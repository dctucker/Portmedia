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
	layout (location = 0) in vec4 in_adsr;
	layout (location = 1) in float in_vol;
	out vec4 adsr;
	out float vol;
	void main() {
		adsr = in_adsr;
		vol  = in_vol;
	}
);
GLSL(geometryShaderSource,

	layout (points) in;
	layout (triangle_strip, max_vertices=12) out;

	in vec4 adsr[];
	in float vol[];
	out vec4 fragColor;

	void main(){
		float v =  max( 0.1, ( 40. + 20. * log( vol[0] )/log(10.0) ) * 0.025 ) ;
		float a =  adsr[0].x;
		float d =  adsr[0].y + a ;
		float s =  adsr[0].z * v; //max( 0.0, ( 40. + 20. * log( adsr[0].z * vol[0] )/log(10.0) ) * 0.025 ) ;
		float r =  adsr[0].w + d ;

		float alpha = 0.4;
		//if( selinst == i ) alpha = 0.6;

		vec2 scaleVector = vec2(1.0, 1.0);

		fragColor = vec4(0.0, 1.0, 1.0, alpha * 1.0 );
		gl_Position = vec4( scaleVector * vec2( 0, 0 ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( a, 0 ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( a, v ), 1.0, 1.0); EmitVertex();
		EndPrimitive();
		
		fragColor = vec4(0.0, 1.0, 1.0, alpha * 0.9 );
		gl_Position = vec4( scaleVector * vec2( a, v ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( d, s ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( a, 0 ), 1.0, 1.0); EmitVertex();
		EndPrimitive();
		
		fragColor = vec4(0.0, 1.0, 1.0, alpha * 1.0 );
		gl_Position = vec4( scaleVector * vec2( a, 0 ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( d, 0 ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( d, s ), 1.0, 1.0); EmitVertex();
		EndPrimitive();

		fragColor = vec4(0.0, 1.0, 1.0, alpha * 0.9 );
		gl_Position = vec4( scaleVector * vec2( d, s ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( d, 0 ), 1.0, 1.0); EmitVertex();
		gl_Position = vec4( scaleVector * vec2( r, 0 ), 1.0, 1.0); EmitVertex();
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
GLfloat adsr[] = {
	0.1,
	0.2,
	1.0,
	0.2,
	0.0
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

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLuint vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(adsr), adsr, GL_STATIC_DRAW);
	/* adsr */
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* volume */
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao);
		glDrawArrays(GL_POINTS, 0, sizeof(adsr) / sizeof(GLfloat) );
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return EXIT_SUCCESS;
}
