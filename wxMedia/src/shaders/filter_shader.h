#pragma once

#include "shader.h"

class FilterShader : public ShaderProgram
{
public:
	mat4 filter_projection = {
		0.5,0,0,0.4,
		0,0.5,0,0.5,
		0,0,1,0,
		0,0,0,1
	};
	GLfloat filter_db[8][128];

	void Setup();
};
