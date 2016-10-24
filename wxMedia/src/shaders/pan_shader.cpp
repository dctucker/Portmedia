#include "pan_shader.h"

void PanShader::Setup()
{
	Defaults();

	MVP.data = mvp.f;
	verts.data = (GLfloat *) pans;
	verts.size = sizeof(pans);

	vertex.source = GLSL(
		uniform mat4 MVP;
		layout (location = 0) in float in_pan;
		layout (location = 1) in float in_alg;
		layout (location = 2) in vec3 in_color;
		out vec3 pos;
		out float pan;
		out float alg;
		out vec3 color;
		void main(){
			pos = vec3( 0.0, gl_VertexID * 1.1, 0.0 );
			//pos = vec3( 0.0, 0.0, 0.0 );
			pan = in_pan;
			alg = in_alg;
			color = in_color;
		}
	);
	geometry.source = GLSL(
		uniform mat4 MVP;
		layout (points) in;
		layout (triangle_strip, max_vertices=8) out;

		in vec3 pos[];
		in float pan[];
		in float alg[];
		in vec3 color[];
		out vec4 fragColor;

		void main(){
			float ampL = clamp( 1.0 - pan[0], 0, 1.0 );
			float ampR = clamp(       pan[0], 0, 1.0 );
			fragColor = vec4( color[0], 0.0 );
			gl_Position = MVP * vec4( pos[0] + vec3( -ampL, 1.0, 1.0 ), 1.0 ); EmitVertex();
			fragColor = vec4( color[0], 0.8 );
			gl_Position = MVP * vec4( pos[0] + vec3(   0.0, 1.0, 1.0 ), 1.0 ); EmitVertex();
			fragColor = vec4( color[0], 0.8 );
			gl_Position = MVP * vec4( pos[0] + vec3( -ampL, 0.0, 1.0 ), 1.0 ); EmitVertex();
			fragColor = vec4( color[0], 0.5 );
			gl_Position = MVP * vec4( pos[0] + vec3(   0.0, 0.0, 1.0 ), 1.0 ); EmitVertex();
			EndPrimitive();
			fragColor = vec4( color[0], 0.0 );
			gl_Position = MVP * vec4( pos[0] + vec3(  ampR, 1.0, 1.0 ), 1.0 ); EmitVertex();
			fragColor = vec4( color[0], 0.8 );
			gl_Position = MVP * vec4( pos[0] + vec3(   0.0, 1.0, 1.0 ), 1.0 ); EmitVertex();
			fragColor = vec4( color[0], 0.8 );
			gl_Position = MVP * vec4( pos[0] + vec3(  ampR, 0.0, 1.0 ), 1.0 ); EmitVertex();
			fragColor = vec4( color[0], 0.5 );
			gl_Position = MVP * vec4( pos[0] + vec3(   0.0, 0.0, 1.0 ), 1.0 ); EmitVertex();
			EndPrimitive();
		}
	);
	fragment.source = default_fragment_source;

	ShaderProgram::Setup();

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 1));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(2);

	verts.draw_size /= 5;
}

