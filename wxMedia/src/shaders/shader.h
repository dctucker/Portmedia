#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define INFOLOG_LEN 512

#define GLSL(shader) ("#version 330 core\n" #shader)
#define HEREDOC(var) #var

#pragma once

union vec2 {
	struct { GLfloat x, y; };
	GLfloat f[2];
};

union vec3 {
	struct { GLfloat x, y, z; };
	struct { GLfloat r, g, b; };
	GLfloat f[3];
};

union vec4 {
	struct { GLfloat x,y,z,w; };
	struct { GLfloat r,g,b,a; };
	GLfloat f[4];
};

union mat4 {
	struct {
		GLfloat
			xx, xy, xz, xw,
			yx, yy, yz, yw,
			zx, zy, zz, zw,
			wx, wy, wz, ww
		;
	};
	struct {
		vec4 x, y, z, w;
	};
	GLfloat f[16];
};

static const GLchar *default_fragment_source = GLSL(
	in vec4 fragColor;
	out vec4 outColor;
	void main() {
		outColor = fragColor;
	}
);

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
	static vec2 global_mouse;

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

