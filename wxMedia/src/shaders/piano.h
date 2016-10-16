_program piano;

void setupPiano(){
	static GLfloat projectionMatrix[16] = {
		2,0,0,-1,
		0,0.25,0,-1,
		0,0,1,0,
		0,0,0,1
	};
	static GLfloat velocities[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f,
	};

	piano.MVP.data = projectionMatrix;
	piano.verts.data = velocities;
	piano.verts.size = sizeof(velocities);
	piano.verts.usage = GL_DYNAMIC_DRAW;

	piano.vertex.source = GLSL(
		layout (location = 0) in float velocity;
		uniform mat4 MVP;
		out float vertColor;
		out int note;
		void main() {
			vertColor = velocity;
			note = int(gl_VertexID) + 21;
		}
	);
	piano.geometry.source = GLSL(
		layout (points) in;
		layout (triangle_strip, max_vertices=40) out;

		uniform mat4 MVP;
		in float vertColor[];
		in int   note[];
		out vec3 fragColor;

		float kloc[13] = float[]( 0, 1.00, 2.00, 2.50, 3.50, 4.45, 5.50, 6.50, 7.00, 8.00, 8.50, 9.50, 10.50 );
		float wloc[13] = float[]( 0, 1.45, 1.55, 2.95, 3.05, 4.45, 5.95, 6.05, 7.45, 7.55, 8.95, 9.05, 10.50 );
		float sep = 0.08;
		float phei = 0.12;
		float alpha = 1.0;
		float aleft = 0.0;
		float arigh = 0.07;
		float atop = 0.0;
		float abot = 0.9;
		float ktop = 1.0;
		float kmid = 0.5;
		float kbot = 0.0;
		float kleft = arigh;
		float krigh = 1.0;
		float n;
		float n1;
		float az  =  0.1;
		float bz  =  0.0;
		float wz  = -0.1;
		float kz0 = -0.1;
		float kz1 =  0.0;
		float nz  = -0.2;
		float cz  = -0.3;
		float spz = -0.3;
		float nw = (krigh-kleft) / 78.0;
		
		void whitekey(in int q, in int p) {
			//gl_Position = gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * vec4(0.004, 0.008, 1.0, 1.0);
			//EmitVertex();
			//EndPrimitive();

			//glNormal3f( 0, 0, -1 )
			gl_Position = MVP * vec4( n + nw * (kloc[q] + sep), ktop, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (kloc[p] - sep), ktop, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (kloc[q] + sep), kmid, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (kloc[p] - sep), kmid, kz0, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( 0, 0, -1 );
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), kmid, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), kmid, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), kbot, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), kbot, kz0, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( 0, 1, 0 );
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), kbot, kz1, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), kbot, kz1, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), kbot, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), kbot, kz0, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( -1, 0, 0 );
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), ktop, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), ktop, kz1, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), kbot, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[p] - sep), kbot, kz1, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( 1, 0, 0 );
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), ktop, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), ktop, kz1, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), kbot, kz0, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * (wloc[q] + sep), kbot, kz1, 1.0); EmitVertex();
			EndPrimitive();
		}

		void blackkey(in int q, in int p) {
			//gl_Position = gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * vec4(0.004, 0.008, 1.0, 1.0);
			//EmitVertex();
			//EndPrimitive();

			//glNormal3f( 0, 0, -1 );
			gl_Position = MVP * vec4( n + nw * kloc[q], ktop, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], ktop, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[q], kmid, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], kmid, nz, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( -1, 0, 0 );
			gl_Position = MVP * vec4( n + nw * kloc[q], ktop, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[q], ktop, wz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[q], kmid, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[q], kmid, wz, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( 1, 0, 0 );
			gl_Position = MVP * vec4( n + nw * kloc[p], ktop, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], ktop, wz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], kmid, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], kmid, wz, 1.0); EmitVertex();
			EndPrimitive();
			//glNormal3f( 0, 1, 0 );
			gl_Position = MVP * vec4( n + nw * kloc[q], kmid, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], kmid, nz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[q], kmid, wz, 1.0); EmitVertex();
			gl_Position = MVP * vec4( n + nw * kloc[p], kmid, wz, 1.0); EmitVertex();
			EndPrimitive();
		}

		void main() {

			float c = vertColor[0];
			int key = note[0] % 12;
			int oct = note[0] / 12;
			n = kleft;
			n += (oct-2) * (nw * 10.5);

			bool white = (key % 2) == (key > 4 ? 1 : 0);
			if( white ){
				fragColor = vec3( 1.0, 1.0 - c, 1.0 - c);
				whitekey(key, key + 1);
			} else {
				fragColor = vec3( 0.3 + c, 0.3 * (1.0 - c), 0.3 * (1.0 - c) );
				blackkey(key, key + 1);
			}
		}
	);
	piano.fragment.source = GLSL(
		in vec3 fragColor;
		out vec4 outColor;
		void main() {
			outColor = vec4( fragColor, 1.0 );
		}
	);
	setupProgram(&piano);

	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
}

