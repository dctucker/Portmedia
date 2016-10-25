#include "filter_shader.h"

void FilterShader::Setup()
{
	Defaults();

	verts.data = filter_db[0];
	verts.size = sizeof(filter_db);
	MVP.data = filter_projection.f;
	verts.usage = GL_DYNAMIC_DRAW;

	vertex.source = GLSL(
		uniform float time;
		uniform mat4 MVP;
		layout (location = 0) in float in_f0;
		layout (location = 1) in float in_f1;
		out float f0;
		out float f1;
		out int index;
		void main() {
			f0 = in_f0;
			f1 = in_f1;
			index = gl_VertexID;
		}
	);
	geometry.source = GLSL(
		uniform float time;
		uniform mat4 MVP;
		layout (points) in;
		layout (triangle_strip, max_vertices=6) out;

		in float f0[];
		in float f1[];
		in int index[];
		out vec4 fragColor;

		const vec3[8] colors = vec3[](
			vec3(1.0, 0.0, 0.0),
			vec3(0.8, 0.8, 8.0),
			vec3(1.0, 1.0, 0.0),
			vec3(0.0, 1.0, 0.0),
			vec3(0.2, 0.3, 1.0),
			vec3(0.0, 1.0, 1.0),
			vec3(1.0, 0.0, 1.0),
			vec3(0.6, 0.5, 0.5)
		);

		void main(){
			float alpha = 0.4;
			int mindb = -36;
			int maxdb = 24;
			int x = 0; //-inst;
			int y = -mindb; //inst * 0.2f;
			int z = 1; //inst / 8.0f;
			vec3 scaleVector = vec3( 1.0/128.0, 1.0/(maxdb-mindb), 1 );
			
			//float a = alpha * (1+0.5*sin(time*3.14)) ;
			float a = 0.8;
			
			//glNormal3f( 0.0, 0.0, 1.0 );

			int i = index[0] % 128;
			int inst = index[0] / 128;

			fragColor = vec4(colors[inst], a );
			//glTranslatef( -0.02, -0.35, 0.0 );

			float f_0 = clamp( f0[0], mindb, maxdb );
			float f_1 = clamp( f1[0], mindb, maxdb );

			gl_Position = MVP * vec4( scaleVector * vec3(   i + x , f_0   + y , z ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec3( i+1 + x , f_1   + y , z ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec3(   i + x , mindb + y , z ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( scaleVector * vec3( i+1 + x , mindb + y , z ), 1.0); EmitVertex();
			EndPrimitive();
		}
	);
	fragment.source = default_fragment_source;

	ShaderProgram::Setup();

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)(1 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	verts.draw_size -= sizeof(GLfloat);
}

