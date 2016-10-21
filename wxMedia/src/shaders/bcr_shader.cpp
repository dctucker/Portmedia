#include "bcr_shader.h"

void BcrShader::Setup()
{
	Defaults();

	MVP.data = bcr_projection.f;
	verts.data = bcr_vertices;
	verts.size = sizeof(bcr_vertices);
	verts.usage = GL_DYNAMIC_DRAW;

	vertex.source = GLSL(
		layout (location = 0) in float inVal;
		uniform mat4 MVP;
		out int   row;
		out int   col;
		out float value;
		void main() {
			row = int(gl_VertexID) / 8;
			col = int(gl_VertexID) % 8;
			value = inVal;
		}
	);
	geometry.source = GLSL(
		layout (points) in;
		layout (triangle_strip, max_vertices=8) out;

		uniform mat4 MVP;
		in int   row[];
		in int   col[];
		in float value[];

		out vec4 fragColor;

		float alpha = 0.8f;
		float bleft = 0.0;
		float brigh = 0.15;
		float btop = 0.6;
		float bbot = 0.8;
		float bwid = brigh - bleft;
		float bhei = bbot - btop;

		void drawValue(in int r, in int c, in float val){

			// zoom into the BCR panel now
			//glTranslatef( bleft, btop, 0.0 );
			//glScalef( bwid, bhei, 1.0 );

			//glPushMatrix();
			// zoom into the current element
			vec3 transVector = vec3( 0.01 + c * 0.120, 0.9 - r * 0.1, 0.0 );
			vec3 scaleVector = vec3( 0.10, 0.0625, 1.0 );

			//glBegin(GL_QUADS);
			//glNormal3f( 0, 0, -1 );

			fragColor = vec4( 1.0, 0.0, 0.0, alpha );
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( 0.0, 0.0, 1.0 ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( 0.0, 1.0, 1.0 ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 0.0, 1.0 ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 1.0, 1.0 ), 1.0); EmitVertex();
		
			fragColor = vec4( 0.1, 0.1, 0.1, alpha );
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 0.0, 1.0 ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( val, 1.0, 1.0 ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( 1.0, 0.0, 1.0 ), 1.0); EmitVertex();
			gl_Position = MVP * vec4( transVector + scaleVector * vec3( 1.0, 1.0, 1.0 ), 1.0); EmitVertex();
			EndPrimitive();
			//glEnd();
			//glPopMatrix();
		}

		void main(){
			drawValue(row[0], col[0], value[0]);

			//drawBoard();
			//drawValues();
		}
	);
	fragment.source = default_fragment_source;

	ShaderProgram::Setup();	

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}
