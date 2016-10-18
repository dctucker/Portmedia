#include "shader.h"

GLfloat ShaderProgram::global_time;
GLfloat ShaderProgram::global_mouse[2];

ShaderProgram::ShaderProgram()
{
	verts.usage = GL_STATIC_DRAW;

	vertex.type = GL_VERTEX_SHADER;
	geometry.type = GL_GEOMETRY_SHADER;
	fragment.type = GL_FRAGMENT_SHADER;

	LoadShaders();
}

void ShaderProgram::Setup()
{
	handle = glCreateProgram();

	Compile(vertex);
	Compile(geometry);
	Compile(fragment);
	Link();

	GetUniforms();
	BindBuffers();
}


void ShaderProgram::Compile(Shader shader)
{
	GLint success;
	GLchar infoLog[INFOLOG_LEN];

	if( shader.source )
	{
		shader.handle = glCreateShader(shader.type);
		glShaderSource(shader.handle, 1, &(shader.source), NULL);
		glCompileShader(shader.handle);
		glGetShaderiv(shader.handle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader.handle, INFOLOG_LEN, NULL, infoLog);
			printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
		}
		glAttachShader(handle, shader.handle);
	}
}

void ShaderProgram::Link()
{
	GLint success;
	GLchar infoLog[INFOLOG_LEN];

	glLinkProgram(handle);
	glGetProgramiv(handle, GL_LINK_STATUS, &success);
	if( ! success)
	{
		glGetProgramInfoLog(handle, INFOLOG_LEN, NULL, infoLog);
		printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}

	if( vertex.source   ) glDeleteShader(vertex.handle);
	if( geometry.source ) glDeleteShader(geometry.handle);
	if( fragment.source ) glDeleteShader(fragment.handle);
}

void ShaderProgram::GetUniforms()
{
	MVP.handle   = glGetUniformLocation(handle,"MVP");
	time.handle  = glGetUniformLocation(handle,"time");
	mouse.handle = glGetUniformLocation(handle,"mouse");
}

void ShaderProgram::BindBuffers()
{
	glGenVertexArrays(1, &(verts.vao));
	glBindVertexArray(verts.vao);
	glGenBuffers(1, &(verts.vbo));
	glBindBuffer(GL_ARRAY_BUFFER, verts.vbo);
	glBufferData(GL_ARRAY_BUFFER, verts.size, verts.data, verts.usage);

	verts.draw_size = verts.size;
	time.data = &( ShaderProgram::global_time );
	mouse.data = ShaderProgram::global_mouse;
}

ShaderProgram::~ShaderProgram()
{
	Teardown();
}

void ShaderProgram::Teardown()
{
	glDeleteVertexArrays(1, &(verts.vao));
	glDeleteBuffers(     1, &(verts.vbo));
}

