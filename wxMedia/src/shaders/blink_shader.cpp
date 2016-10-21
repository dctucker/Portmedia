#include "blink_shader.h"

void BlinkShader::Setup()
{
	Defaults();

	MVP.data = mvp.f;
	verts.data = (GLfloat *) blinks.amplfo;
	verts.size = sizeof(blinks);

	vertex.source = GLSL(
		uniform mat4 MVP;
		layout (location = 0) in vec2 in_pos;
		layout (location = 1) in vec3 in_afp;
		layout (location = 2) in vec3 in_color;
		out vec3 pos;
		out vec3 afp;
		out vec3 color;
		void main(){
			pos = vec3( in_pos, 1.0 );
			afp = in_afp;
			color = in_color;
		}
	);
	geometry.source = GLSL(
		uniform mat4 MVP;
		uniform float time;
		layout (points) in;
		layout (triangle_strip, max_vertices=4) out;

		in vec3 pos[];
		in vec3 afp[];
		in vec3 color[];
		out vec4 fragColor;
		void main(){
			fragColor = vec4( color[0], mod(time * afp[0].y, 1.0) );
			gl_Position = MVP * vec4( pos[0].x-0.5, pos[0].y+0.5, 1.0, 1.0 ); EmitVertex();
			gl_Position = MVP * vec4( pos[0].x+0.5, pos[0].y+0.5, 1.0, 1.0 ); EmitVertex();
			gl_Position = MVP * vec4( pos[0].x-0.5, pos[0].y-0.5, 1.0, 1.0 ); EmitVertex();
			gl_Position = MVP * vec4( pos[0].x+0.5, pos[0].y-0.5, 1.0, 1.0 ); EmitVertex();
			EndPrimitive();
		}
	);
	fragment.source = default_fragment_source;

	ShaderProgram::Setup();
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 5));
	glEnableVertexAttribArray(2);

	verts.draw_size /= 8;
}
