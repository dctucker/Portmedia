#pragma once

#include "shader.h"

class BiquadShader : public ShaderProgram
{
public:
	mat4 projectionMatrix = {
		0.5,0,0,0.4,
		0,0.5,0,0.5,
		0,0,1,0,
		0,0,0,1
	};
	union BIQUAD {
		struct {
			GLfloat a0, a1, a2, b0, b1, b2, r, g, b;
		};
		GLfloat f[9];
	} biquad_coeffs[8] = {
		// a0, a1, a2, b0, b1, b2, r, g, b
		{0,0,0,0,0,0,  1.0, 0.0, 0.0},
		{0,0,0,0,0,0,  0.8, 0.8, 8.0},
		{0,0,0,0,0,0,  1.0, 1.0, 0.0},
		{0,0,0,0,0,0,  0.0, 1.0, 0.0},
		{0,0,0,0,0,0,  0.2, 0.3, 1.0},
		{0,0,0,0,0,0,  0.0, 1.0, 1.0},
		{0,0,0,0,0,0,  1.0, 0.0, 1.0},
		{0,0,0,0,0,0,  0.6, 0.5, 0.5},
	};

	void Setup();
};
