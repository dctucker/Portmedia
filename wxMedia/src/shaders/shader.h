#pragma once

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define INFOLOG_LEN 512

#define GLSL(shader) ("#version 330 core\n" #shader)
#define HEREDOC(var) #var


class ShaderProgram
{
private:
	GLint handle;
	void Compile();
	void Link();

public:
	struct Shader {
		GLint handle;
		GLuint type;
		const GLchar *source;
	} vertex, geometry, fragment;

	struct Uniform {
		GLint handle;
		const GLfloat *data;
	} MVP, time, mouse;

	struct {
		GLuint size;
		GLuint draw_size;
		GLfloat *data;
		GLuint vao;
		GLuint vbo;
		GLuint usage;
	} verts;

	static GLfloat global_time;
	static GLfloat global_mouse[2];

	ShaderProgram();
	void Setup();

	void Compile(Shader);
	virtual void LoadShaders(){}
	void GetUniforms();
	void BindBuffers();
	virtual void SetAttribs(){}

	void Run(bool copy=false);

	~ShaderProgram();
	void Teardown();
};

inline void ShaderProgram::Run(bool copy)
{
	glUseProgram(handle);
	glUniform1f(time.handle , *(time.data));
	glUniform2f(mouse.handle, mouse.data[0], mouse.data[1]);
	glUniformMatrix4fv(MVP.handle, 1, GL_TRUE, &(MVP.data[0]));
	if( copy )
	{
		glBindBuffer(GL_ARRAY_BUFFER, verts.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size, verts.data);
	}
	glBindVertexArray(verts.vao);
	glDrawArrays(GL_POINTS, 0, verts.draw_size / sizeof(GLfloat) );
	glBindVertexArray(0);
}

