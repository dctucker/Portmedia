#include "shader.h"

class AdsrShader : public ShaderProgram
{
public:
	Mat44 mvp = {
		0.5,0,0,-0.3,
		0,0.5,0,0.5,
		0,0,1,0,
		0,0,0,1
	};
	GLfloat adsrVerts[64] = {
	//   A     D    S    R    V    R    G    B
		0.10, 0.2, 1.0, 0.5, 0.5, 1.0, 0.0, 0.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 0.8, 0.8, 8.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 1.0, 1.0, 0.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 0.0, 1.0, 0.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 0.2, 0.3, 1.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 0.0, 1.0, 1.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 1.0, 0.0, 1.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 0.6, 0.5, 0.5,
	};
	struct ADSR {
		GLfloat a, d, s, r, v;
		struct Color {
			GLfloat r, g, b;
		};
	};
	ADSR *adsrs[8];

	void Setup();
	void SetAttribs();
};
