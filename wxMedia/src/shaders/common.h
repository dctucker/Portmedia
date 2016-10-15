#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#define INFOLOG_LEN 512

#define GLSL(shader) ("#version 330 core\n" #shader)
#define HEREDOC(var) #var

typedef struct _program {
	GLint handle;
	struct {
		GLint handle;
		const GLchar *source;
	} vertex, geometry, fragment;
	struct {
		GLint handle;
		const GLfloat *data;
	} MVP, time, mouse;
	struct {
		GLuint size;
		GLuint draw_size;
		GLfloat *data;
		GLuint vao;
		GLuint vbo;
	} verts;
} _program;

GLfloat global_time = 0.0f;
GLfloat mouse[2];

void setupProgram(_program *program){
	GLint success;
	GLchar infoLog[INFOLOG_LEN];

	program->handle = glCreateProgram();

	if( program->vertex.source ){
		program->vertex.handle = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(program->vertex.handle, 1, &(program->vertex.source), NULL);
		glCompileShader(program->vertex.handle);
		glGetShaderiv(program->vertex.handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(program->vertex.handle, INFOLOG_LEN, NULL, infoLog);
			printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
		}
		glAttachShader(program->handle, program->vertex.handle);
	}
	if( program->geometry.source ){
		program->geometry.handle = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(program->geometry.handle, 1, &(program->geometry.source), NULL);
		glCompileShader(program->geometry.handle);
		glGetShaderiv(program->geometry.handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(program->geometry.handle, INFOLOG_LEN, NULL, infoLog);
			printf("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n%s\n", infoLog);
		}
		glAttachShader(program->handle, program->geometry.handle);
	}
	if( program->fragment.source ){
		program->fragment.handle = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(program->fragment.handle, 1, &(program->fragment.source), NULL);
		glCompileShader(program->fragment.handle);
		glGetShaderiv(program->fragment.handle, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(program->fragment.handle, INFOLOG_LEN, NULL, infoLog);
			printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
		}
		glAttachShader(program->handle, program->fragment.handle);
	}

	glLinkProgram(program->handle);
	glGetProgramiv(program->handle, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program->handle, INFOLOG_LEN, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}
	program->MVP.handle = glGetUniformLocation(program->handle,"MVP");
	program->time.handle = glGetUniformLocation(program->handle,"time");
	program->mouse.handle = glGetUniformLocation(program->handle,"mouse");

	if( program->vertex.source   ) glDeleteShader(program->vertex.handle);
	if( program->geometry.source ) glDeleteShader(program->geometry.handle);
	if( program->fragment.source ) glDeleteShader(program->fragment.handle);

	glGenVertexArrays(1, &(program->verts.vao));
	glGenBuffers(1, &(program->verts.vbo));
	glBindVertexArray(program->verts.vao);
	glBindBuffer(GL_ARRAY_BUFFER, program->verts.vbo);
	glBufferData(GL_ARRAY_BUFFER, program->verts.size, program->verts.data, GL_STATIC_DRAW);

	program->verts.draw_size = program->verts.size;
	program->time.data = &global_time;
	program->mouse.data = mouse;
}

void teardownProgram(_program *program){
	glDeleteVertexArrays(1, &(program->verts.vao));
	glDeleteBuffers(     1, &(program->verts.vbo));
}

inline static void runProgram(_program *program){
	glUseProgram(program->handle);
	glUniform1f(program->time.handle , *(program->time.data));
	glUniform2f(program->mouse.handle, program->mouse.data[0], program->mouse.data[1]);
	glUniformMatrix4fv(program->MVP.handle, 1, GL_TRUE, &(program->MVP.data[0]));
	glBindVertexArray(program->verts.vao);
	glDrawArrays(GL_POINTS, 0, program->verts.draw_size / sizeof(GLfloat) );
}
