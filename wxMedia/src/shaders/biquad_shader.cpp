#include "biquad_shader.h"

void BiquadShader::Setup()
{
	Defaults();

	verts.data = biquad_coeffs;
	verts.size = sizeof(biquad_coeffs);
	MVP.data = projectionMatrix;
	verts.usage = GL_DYNAMIC_DRAW;

	vertex.source = GLSL(
		uniform float time;
		uniform mat4 MVP;
		layout (location = 0) in float ab[6];
		//layout (location = 1) in vec3 in_color;
		out float a0;
		out float a1;
		out float a2;
		out float b0;
		out float b1;
		out float b2;
		out int index;
		out vec3 color;
		void main() {
			a0 = ab[0];
			a1 = ab[1];
			a2 = ab[2];
			b0 = ab[3];
			b1 = ab[4];
			b2 = ab[5];
			index = gl_VertexID;
			color = vec3(1,0,0);//in_color;
		}
	);
	geometry.source = GLSL(
		uniform float time;
		uniform mat4 MVP;
		layout (points) in;
		layout (triangle_strip, max_vertices=256) out;

		in float a0[];
		in float a1[];
		in float a2[];
		in float b0[];
		in float b1[];
		in float b2[];
		in int index[];
		in vec3 color[];
		out vec4 fragColor;

		float height(float f){
			float w = 2.f * 3.141592653 * f / 96000.0;
			float phi = sin(0.5f * w);
			float b024 = 4.f * b0[0] * b2[0];
			float a024 = 4.f * a0[0] * a2[0];
			float b012s = b0[0] + b1[0] + b2[0];
			float a012s = a0[0] + a1[0] + a2[0];
			b012s *= b012s;
			a012s *= a012s;
			phi *= phi;
			float phi2 = phi * phi;

			return 10.f * (
				log( b012s + 4.f * ( b024 * phi2 - (b0[0]*b1[0] + b024 + b1[0]*b2[0]) * phi ) ) -
				log( a012s + 4.f * ( a024 * phi2 - (a0[0]*a1[0] + a024 + a1[0]*a2[0]) * phi ) )
			);
		}

		void main(){
			float alpha = 0.4;
			int mindb = -36;
			int maxdb = 24;
			int x = 0; //-inst;
			int y = -mindb; //inst * 0.2f;
			int z = 1; //inst / 8.0f;
			int i = 0;
			vec3 scaleVector = vec3( 1.0/128.0, 1.0/(maxdb-mindb), 1 );
			
			fragColor = vec4(0.0, 1.0, 0.0, alpha * (1+0.5*sin(time*3.14)) );
			
			//int i = index[0];
			float f_0 = -36;
			float f_1 = -36;
			for(float f = 15.625f; f < 18000; f *= 2 ){
				for( int g = 0; g < 12 && i < 128; g++ ){
					float ef = f * pow(2., g / 12.f);
					float f_0 = clamp( height(ef), mindb, maxdb );

					gl_Position = MVP * vec4( scaleVector * vec3(   i + x , f_1   + y , z ), 1.0); EmitVertex();
					gl_Position = MVP * vec4( scaleVector * vec3( i+1 + x , f_0   + y , z ), 1.0); EmitVertex();
					gl_Position = MVP * vec4( scaleVector * vec3(   i + x , mindb + y , z ), 1.0); EmitVertex();
					gl_Position = MVP * vec4( scaleVector * vec3( i+1 + x , mindb + y , z ), 1.0); EmitVertex();

					f_1 = f_0;
					++i;
				}
			}
		}
	);
	fragment.source = GLSL(
		in vec4 fragColor;
		out vec4 outColor;
		void main() {
			outColor = fragColor;
		}
	);


	ShaderProgram::Setup();

	glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	verts.draw_size /= 9;
}
