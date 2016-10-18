#include "scope_shader.h"

void ScopeShader::LoadShaders()
{
	MVP.data = projectionMatrix;
	verts.data = min_max;
	verts.size = sizeof(min_max);
	verts.usage = GL_STREAM_DRAW;
	
	vertex.source = GLSL(
		layout (location = 0) in vec2 in_min_max_0;
		layout (location = 1) in vec2 in_min_max_1;
		uniform mat4 MVP;
		out vec2 min_max_0;
		out vec2 min_max_1;
		out int index;
		void main() {
			min_max_0 = in_min_max_0;
			min_max_1 = in_min_max_1;
			index = gl_VertexID;
		}
	);
	geometry.source = GLSL(

		layout (points) in;
		layout (triangle_strip, max_vertices=6) out;

		uniform mat4 MVP;
		uniform float time;
		in vec2 min_max_0[];
		in vec2 min_max_1[];
		in int index[];
		out vec4 fragColor;

		void main(){
			float alpha = 0.75;
			float height = 0.5;
			float width = 349.0;
			float sleft = 0.0;
			float srigh = sleft + 1.0;
			//float stop = 1.0;
			//float sbot = stop - height;
			float center_y = 0.0;
			float dx = (1.0 / width) * (srigh - sleft);
			float z = 2.0;
			
			//glTranslatef( 0, 0, 0.9 );
			//glBegin(GL_POLYGON);
			//glNormal3f( 0, 0, -1 );
			/*
			glColor4f( 0.1, 0.2, 0.1, alpha );
			glVertex2f( sleft, stop );
			glVertex2f( sleft, sbot );
			glVertex2f( srigh, sbot );
			glVertex2f( srigh, stop );
			*/
			//glEnd();
			
			//glNormal3f( 0.0, 0.0, 1.0 );
			float t = 0;
			mat4 rot = mat4(
				1, 0, 0, 0.0,
				0, cos(2*t), -sin(2*t), 0.0,
				0, sin(2*t), cos(2*t), 0,
				0, 0, 0, 1
			);

			int i = index[0];
			float scope_minv_0 = min_max_0[0].x;
			float scope_minv_1 = min_max_1[0].x;
			float scope_maxv_0 = min_max_0[0].y;
			float scope_maxv_1 = min_max_1[0].y;

			fragColor = vec4(0.6, 1.0 ,0.6, alpha );
			gl_Position = rot * MVP * vec4(     i * dx + sleft, center_y - height * scope_maxv_0, z, 1.0 ); EmitVertex();
			gl_Position = rot * MVP * vec4( (i+1) * dx + sleft, center_y - height * scope_maxv_1, z, 1.0 ); EmitVertex();
			fragColor = vec4(0.6, 1.0 ,0.6, 0.1 );
			gl_Position = rot * MVP * vec4(     i * dx + sleft, center_y, z, 1.0 ); EmitVertex();
			gl_Position = rot * MVP * vec4( (i+1) * dx + sleft, center_y, z, 1.0 ); EmitVertex();
			fragColor = vec4(0.6, 1.0 ,0.6, alpha );
			gl_Position = rot * MVP * vec4(     i * dx + sleft, center_y - height * scope_minv_0, z, 1.0 ); EmitVertex();
			gl_Position = rot * MVP * vec4( (i+1) * dx + sleft, center_y - height * scope_minv_1, z, 1.0 ); EmitVertex();
			EndPrimitive();
		}
	);
	fragment.source = GLSL(
		in vec4 fragColor;
		out vec4 outColor;
		void main() {
			outColor = fragColor;
		}
	);
}

void ScopeShader::SetAttribs()
{
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//verts.draw_size -= sizeof(GLfloat);
	verts.draw_size /= 2;
}