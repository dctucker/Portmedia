#pragma once

#include "shader.h"

class BiquadShader : public ShaderProgram
{
	GLfloat projectionMatrix[16] = {
		0.5,0,0,0.4,
		0,0.5,0,0.5,
		0,0,1,0,
		0,0,0,1
	};
	GLfloat biquad_coeffs[72] = {
		// a0, a1, a2, b0, b1, b2, r, g, b
		0,0,0,0,0,0,  1.0, 0.0, 0.0,
		0,0,0,0,0,0,  0.8, 0.8, 8.0,
		0,0,0,0,0,0,  1.0, 1.0, 0.0,
		0,0,0,0,0,0,  0.0, 1.0, 0.0,
		0,0,0,0,0,0,  0.2, 0.3, 1.0,
		0,0,0,0,0,0,  0.0, 1.0, 1.0,
		0,0,0,0,0,0,  1.0, 0.0, 1.0,
		0,0,0,0,0,0,  0.6, 0.5, 0.5,
	};

	void LoadShaders();
	void SetAttribs();
};
