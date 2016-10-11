_program bcr;

void setupBcr(){
	static GLfloat projectionMatrix[16] = {
		0.5,0,0,-0.95,
		0,0.5,0, 0.45,
		0,0,1,0,
		0,0,0,1
	};
	static GLfloat vertices[] = {
		0.11, 0.12, 0.13, 0.14, 0.15, 0.16, 0.17, 0.18,
		0.21, 0.22, 0.23, 0.24, 0.25, 0.26, 0.27, 0.28,
		0.31, 0.32, 0.33, 0.34, 0.35, 0.36, 0.37, 0.38,
		0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48,
		0.41, 0.42, 0.43, 0.44, 0.45, 0.46, 0.47, 0.48,
		0.51, 0.52, 0.53, 0.54, 0.55, 0.56, 0.57, 0.58,
		0.61, 0.62, 0.63, 0.64, 0.65, 0.66, 0.67, 0.68,
		0.71, 0.72, 0.73, 0.74, 0.75, 0.76, 0.77, 0.78,
		0.91, 0.92, 0.93, 0.94, 0.95, 0.96, 0.99, 1.00
	};
	bcr.MVP.data = projectionMatrix;
	bcr.verts.data = vertices;
	bcr.verts.size = sizeof(vertices);

	bcr.vertex.source = GLSL(
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
	bcr.geometry.source = GLSL(
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
	bcr.fragment.source = GLSL(
		in vec4 fragColor;
		out vec4 outColor;

		void main() {
			outColor = fragColor;
		}
	);

	setupProgram(&bcr);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}
