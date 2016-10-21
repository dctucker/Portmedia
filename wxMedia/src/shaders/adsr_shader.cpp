#include "adsr_shader.h"

void AdsrShader::Setup()
{
	Defaults();

	MVP.data = mvp.f;
	verts.data = (GLfloat *) adsrs;
	verts.size = sizeof(adsrs);

	vertex.source = GLSL(
		uniform mat4 MVP;
		layout (location = 0) in vec4 in_adsr;
		layout (location = 1) in float in_vol;
		layout (location = 2) in vec3 in_color;
		out vec4 adsr;
		out float vol;
		out vec3 color;
		void main() {
			adsr = in_adsr;
			vol  = in_vol;
			color = in_color;
		}
	);
	geometry.source = GLSL(
		uniform mat4 MVP;
		layout (points) in;
		layout (triangle_strip, max_vertices=12) out;

		in vec4 adsr[];
		in float vol[];
		in vec3 color[];
		out vec4 fragColor;

		void main(){
			float v =  clamp( (40. + 20. * log( vol[0] )/log(10.0) ) * 0.025, 0.1, 1.0 ) ;
			float a =  clamp( adsr[0].x, 0, 1);
			float d =  clamp( adsr[0].y + a, 0, 1);
			float s =  clamp( adsr[0].z * v, 0, 1); //max( 0.0, ( 40. + 20. * log( adsr[0].z * vol[0] )/log(10.0) ) * 0.025 ) ;
			float r =  clamp( adsr[0].w + d, 0, 1);
			float r0=  clamp( adsr[0].w + d - 1, 0, s);

			float alpha = 0.4;
			//if( selinst == i ) alpha = 0.6;

			vec2 scaleVector = vec2(1.0, 1.0);

			fragColor = vec4(color[0], alpha * 1.0 );
			gl_Position = MVP * vec4( scaleVector * vec2( 0, 0 ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( a, 0 ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( a, v ), 1.0, 1.0); EmitVertex();
			fragColor = vec4(color[0], alpha * 0.7 );
			//gl_Position = MVP * vec4( scaleVector * vec2( a, v ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( d, s ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( a, 0 ), 1.0, 1.0); EmitVertex();
			fragColor = vec4(color[0], alpha * 0.9 );
			//gl_Position = MVP * vec4( scaleVector * vec2( a, 0 ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( d, 0 ), 1.0, 1.0); EmitVertex();
			fragColor = vec4(color[0], alpha * 0.9 );
			gl_Position = MVP * vec4( scaleVector * vec2( d, s ), 1.0, 1.0); EmitVertex();
			//gl_Position = MVP * vec4( scaleVector * vec2( d, s ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( d, 0 ), 1.0, 1.0); EmitVertex();
			fragColor = vec4(color[0], alpha * 0.5 );
			gl_Position = MVP * vec4( scaleVector * vec2( r, r0 ), 1.0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec2( r, 0 ), 1.0, 1.0); EmitVertex();
			EndPrimitive();
		}
	);
	fragment.source = default_fragment_source;
	
	ShaderProgram::Setup();

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	verts.draw_size /= 8;
}
