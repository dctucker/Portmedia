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
public:
	GLint handle;

	void Compile();
	void Link();

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
	void Defaults();
	virtual void Setup() = 0;

	void Compile(Shader);
	void GetUniforms();
	void BindBuffers();

	void Run(bool copy=false);

	~ShaderProgram();
	void Teardown();
};

