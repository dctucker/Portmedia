#include "shader.h"

GLfloat ShaderProgram::global_time;
GLfloat ShaderProgram::global_mouse[2];

ShaderProgram::ShaderProgram()
{
	//Defaults();
}

void ShaderProgram::Defaults()
{
	verts.usage = GL_DYNAMIC_DRAW;

	vertex.type = GL_VERTEX_SHADER;
	geometry.type = GL_GEOMETRY_SHADER;
	fragment.type = GL_FRAGMENT_SHADER;

	vertex.source = NULL;
	geometry.source = NULL;
	fragment.source = NULL;

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
	//time.data = &( ShaderProgram::global_time );
	//mouse.data = ShaderProgram::global_mouse;
}



void ShaderProgram::Run(bool copy)
{
	glUseProgram(handle);
	//glUniform1f(time.handle , *(time.data));
	//glUniform2f(mouse.handle, mouse.data[0], mouse.data[1]);
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


ShaderProgram::~ShaderProgram()
{
	Teardown();
}

void ShaderProgram::Teardown()
{
	glDeleteVertexArrays(1, &(verts.vao));
	glDeleteBuffers(     1, &(verts.vbo));
}

