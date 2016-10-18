#pragma once

#include "shader.h"

class ScopeShader : public ShaderProgram
{
	GLfloat projectionMatrix[16] = {
		1.80, 0.00, 0, -0.9,
		0.00, 2.00, 0,  0.0,
		0.00, 0.00, 1, -2.0,
		0.00, 0.00, 0,  1.0
	};
	GLfloat min_max[698] = {
		 0.0, 0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9,
		-0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0,  0.0, 0.1,  0.1,
		 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7,
		 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3,
		-0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5,
		-0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5,
		 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3,
		 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7,
		-0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1,
		 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 1.0,  1.0,
		 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1,
		-0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -1.0, -1.0, -0.8, -0.8, -0.7, -0.7,
		-0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3,
		 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5,
		 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5,
		-0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3,
		-0.2, -0.2, -0.1, -0.1, 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7,
		 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1,
		 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9,
		-0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0,  0.0, 0.1,  0.1,
		 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7,
		 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3,
		-0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5,
		-0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5,
		 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3,
		 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7,
		-0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1,
		 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3, 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9,
		 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5, 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1,
		-0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5, -0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7,
		-0.6, -0.6, -0.5, -0.5, -0.4, -0.4, -0.3, -0.3, -0.2, -0.2, -0.1, -0.1, 0.0,  0.0, -0.1, -0.9, -0.2, -0.8, -0.3, -0.7,
		-0.4, -0.6, -0.5, -0.5, -0.6, -0.4, -0.7, -0.3, -0.8, -0.2, -0.9, -0.1, 0.0,  0.0, 0.1,  0.1, 0.2,  0.2, 0.3,  0.3,
		 0.4,  0.4, 0.5,  0.5, 0.6,  0.6, 0.7,  0.7, 0.8,  0.8, 0.9,  0.9, 0.8,  0.8, 0.7,  0.7, 0.6,  0.6, 0.5,  0.5,
		 0.4,  0.4, 0.3,  0.3, 0.2,  0.2, 0.1,  0.1, 0.0,  0.0, -0.1, -0.1, -0.2, -0.2, -0.3, -0.3, -0.4, -0.4, -0.5, -0.5,
		-0.6, -0.6, -0.7, -0.7, -0.8, -0.8, -0.9, -0.9, -0.8, -0.8, -0.7, -0.7, -0.6, -0.6, -0.5, -0.5
	};

	void LoadShaders();
	void SetAttribs();
};