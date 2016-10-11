#include "common.h"

_program adsr, piano, led, bcr;


void setupAdsr(){
	static GLfloat adsrMVP[16] = {
		0.5,0,0,-0.5,
		0,0.5,0,-0.5,
		0,0,1,0,
		0,0,0,1
	};
	static GLfloat adsrVerts[] = {
	//   A     D    S    R    V    R    G    B
		0.10, 0.2, 1.0, 0.5, 0.5, 1.0, 0.0, 0.0,
		0.05, 0.1, 0.7, 0.1, 0.1, 0.0, 1.0, 1.0,
	};

	adsr.MVP.data = adsrMVP;
	adsr.verts.data = adsrVerts;
	adsr.verts.size = sizeof(adsrVerts);

	adsr.vertex.source = GLSL(
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
	adsr.geometry.source = GLSL(
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
			float r0=  clamp( adsr[0].w + d - 1, 0, 1);

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
	adsr.fragment.source = GLSL(
		in vec4 fragColor;
		out vec4 outColor;
		void main() {
			outColor = fragColor;
		}
	);
	setupProgram(&adsr);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

}

void setupPiano(){
	static GLfloat projectionMatrix[16] = {
		2,0,0,-1,
		0,0.25,0,-1,
		0,0,1,0,
		0,0,0,1
	};
	static GLfloat velocities[] = {
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.9f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.6f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f,
	};

	piano.MVP.data = projectionMatrix;
	piano.verts.data = velocities;
	piano.verts.size = sizeof(velocities);

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

void setupLed(){
	static GLfloat projectionMatrix[16] = {
		0.10, 0.00, 0.00, 0.00,
		0.00, 0.20, 0.00, 0.00,
		0.00, 0.00, 1.00, 0.00,
		0.00, 0.00, 0.00, 1.00
	};
	static GLfloat vertices[] = {
		/*   Positions            Colors */
		-10.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 'A',
	/*
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 'A',
		-0.4f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 'R',
		-0.3f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 'E',
		-0.2f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 'L',
		-0.1f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 'I',
		 0.0f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, ' ',
		 0.1f, -0.5f, 0.0f,   1.0f, 0.4f, 0.4f, '\177',
		 0.2f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f, ' ',
		 0.3f, -0.5f, 0.0f,   0.0f, 0.6f, 1.0f, 'C',
		 0.4f, -0.5f, 0.0f,   0.0f, 0.6f, 1.0f, 'A',
		 0.5f, -0.5f, 0.0f,   0.0f, 0.6f, 1.0f, 'S',
		 0.6f, -0.5f, 0.0f,   0.0f, 0.6f, 1.0f, 'E',
		 0.7f, -0.5f, 0.0f,   0.0f, 0.6f, 1.0f, 'Y'
	*/
	};
	led.MVP.data = projectionMatrix;
	led.verts.data = vertices;
	led.verts.size = sizeof(vertices);

	led.vertex.source = GLSL(
		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 color;
		layout (location = 2) in float fChar;
		uniform mat4 MVP;
		out vec3 vertColor;
		out int character;
		void main() {
			gl_Position = vec4(position + vec3(0.5,0.5, 0.0), 1.0f);
			vertColor = color;
			character = int(fChar);
		}
	);
	led.geometry.source = GLSL(
		const int led16_vertex_data[] = int[](
			// top left
			-9,  9,
			-8, 10,
			-8,  8,
			-1, 10,
			-1,  8,
			 0,  9,
			// top right
			 0,  9,
			 1, 10,
			 1,  8,
			 8, 10,
			 8,  8,
			 9,  9,
			// upper right
			 8,  1,
			 8,  8,
			 9,  0,
			 9,  9,
			10,  1,
			10,  8,
			// lower right
			 8, -1,
			 8, -8,
			 9,  0,
			 9, -9,
			10, -1,
			10, -8,
			// bottom right
			 0,  -9,
			 1, -10,
			 1,  -8,
			 8, -10,
			 8,  -8,
			 9,  -9,
			// bottom left
			-9,  -9,
			-8, -10,
			-8,  -8,
			-1, -10,
			-1,  -8,
			 0,  -9,
			// lower left
			-8,  -1,
			-8,  -8,
			-9,   0,
			-9,  -9,
			-10, -1,
			-10, -8,
			// upper left
			-10, 8,
			-10, 1,
			-9,  9,
			-9,  0,
			-8,  8,
			-8,  1,
			// diagonal upper left
			 0, 0,
			-1, 1,
			-1, 2,
			-8, 7,
			-7, 8,
			-8, 8,
			// top center
			-1, 2,
			-1, 8,
			 0, 0,
			 0, 9,
			 1, 2,
			 1, 8,
			// diagonal upper right
			 0, 0,
			 1, 2,
			 1, 1,
			 7, 8,
			 8, 7,
			 8, 8,
			// right middle
			 0,  0,
			 2, -1,
			 2,  1,
			 8, -1,
			 8,  1,
			 9,  0,
			// diagonal lower right
			 0,  0,
			 1, -1,
			 1, -2,
			 8, -7,
			 7, -8,
			 8, -8,
			// bottom center
			-1, -2,
			-1, -8,
			 0,  0,
			 0, -9,
			 1, -2,
			 1, -8,
			// diagonal lower left
			 0,  0,
			-1, -2,
			-1, -1,
			-7, -8,
			-8, -7,
			-8, -8,
			// left middle
			 0,  0,
			-2,  1,
			-2, -1,
			-8,  1,
			-8, -1,
			-9,  0
		);
		/*
			 ---0-|-1---  
			7 \   9   / 2 
			|  8\ | /a  | 
			--f--- ---b-- 
			|  e/ | \c  | 
			6 /   d   \ 3 
			 ---5-|-4---  
		*/
		const int led16_ee[] = int[](
			0x60f0, // NUL \0
			0x4001, // SOH
			0x4002, // STX
			0x49c4, // ETX ^C
			0x4004, // EOT
			0x4005, // ENQ
			0x4006, // ACK
			0x60b0, // BEL \a
			0x20f0, // BS \b
			0x6080, // TAB \t
			0x60c0, // LF \n
			0x00e0, // VT \v
			0x40d4, // NP \f
			0x40c0, // CR \r
			0x400e, // SO
			0x400f, // SI ^O
			0x4010, // DLE
			0x4011, // DC1
			0x4012, // DC2
			0x4013, // DC3
			0x4014, // DC4
			0x4015, // NAK
			0x4016, // SYN
			0x4017, // ETB
			0x4018, // CAN
			0x4019, // EM
			0x41b0, // EOF SUB ^Z
			0x40f0, // ESC \e
			0x401c, // FS
			0x401d, // GS
			0x401e, // RS
			0x401f, // US
					//   0 1 2 3  4 5 6 7  8 9 a b  c d e f    //
			0x0000, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }, // 
			0x8541, // { 1,0,0,0, 0,1,0,1, 0,1,0,0, 0,0,0,1 }, // 
			0x0140, // { 0,0,0,0, 0,0,0,1, 0,1,0,0, 0,0,0,0 }, //"
			0x3c55, // { 0,0,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,1 }, //#
			0xdd55, // { 1,1,0,1, 1,1,0,1, 0,1,0,1, 0,1,0,1 }, //$
			0x9977, // { 1,0,0,1, 1,0,0,1, 0,1,1,1, 0,1,1,1 }, //%
			0x875d, // { 1,0,0,0, 0,1,1,1, 0,1,0,1, 1,1,0,1 }, //&
			0x0040, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,0,0,0 }, //'
			0x0028, // { 0,0,0,0, 0,0,0,0, 0,0,1,0, 1,0,0,0 }, //(
			0x0082, // { 0,0,0,0, 0,0,0,0, 1,0,0,0, 0,0,1,0 }, //)
			0x00ff, // { 0,0,0,0, 0,0,0,0, 1,1,1,1, 1,1,1,1 }, //*
			0x0055, // { 0,0,0,0, 0,0,0,0, 0,1,0,1, 0,1,0,1 }, //+
			0x0003, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1 }, //,
			0x0011, // { 0,0,0,0, 0,0,0,0, 0,0,0,1, 0,0,0,1 }, //-
			0x0400, // { 0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0 }, //.
			0x0022, // { 0,0,0,0, 0,0,0,0, 0,0,1,0, 0,0,1,0 }, ///
			0xff22, // { 1,1,1,1, 1,1,1,1, 0,0,1,0, 0,0,1,0 }, //0
			0x8c44, // { 1,0,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //1
			0xee11, // { 1,1,1,0, 1,1,1,0, 0,0,0,1, 0,0,0,1 }, //2
			0xfc11, // { 1,1,1,1, 1,1,0,0, 0,0,0,1, 0,0,0,1 }, //3
			0x3111, // { 0,0,1,1, 0,0,0,1, 0,0,0,1, 0,0,0,1 }, //4
			0xdc90, // { 1,1,0,1, 1,1,0,0, 1,0,0,1, 0,0,0,0 }, //5
			0xdf11, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //6
			0xc023, // { 1,1,0,0, 0,0,0,0, 0,0,1,0, 0,0,1,1 }, //7
			0xff11, // { 1,1,1,1, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //8
			0xfd11, // { 1,1,1,1, 1,1,0,1, 0,0,0,1, 0,0,0,1 }, //9
			0x8400, // { 1,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,0,0 }, //:
			0x8003, // { 1,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,1,1 }, //;
			0x0c22, // { 0,0,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //<
			0x0c11, // { 0,0,0,0, 1,1,0,0, 0,0,0,1, 0,0,0,1 }, //=
			0x0c88, // { 0,0,0,0, 1,1,0,0, 1,0,0,0, 1,0,0,0 }, //>
			0xe014, // { 1,1,1,0, 0,0,0,0, 0,0,0,1, 0,1,0,0 }, //?
			0xfe05, // { 1,1,1,1, 1,1,1,0, 0,0,0,0, 0,1,0,1 }, //@
			0x3032, // { 0,0,1,1, 0,0,0,0, 0,0,1,1, 0,0,1,0 }, //A
			0xfc54, // { 1,1,1,1, 1,1,0,0, 0,1,0,1, 0,1,0,0 }, //B
			0xcf00, // { 1,1,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //C
			0xfc44, // { 1,1,1,1, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //D
			0xcf11, // { 1,1,0,0, 1,1,1,1, 0,0,0,1, 0,0,0,1 }, //E
			0xc311, // { 1,1,0,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //F
			0xdf10, // { 1,1,0,1, 1,1,1,1, 0,0,0,1, 0,0,0,0 }, //G
			0x3311, // { 0,0,1,1, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //H
			0xcc44, // { 1,1,0,0, 1,1,0,0, 0,1,0,0, 0,1,0,0 }, //I
			0xc644, // { 1,1,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,0 }, //J
			0x0329, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 1,0,0,1 }, //K
			0x0f00, // { 0,0,0,0, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //L
			0x33a4, // { 0,0,1,1, 0,0,1,1, 1,0,1,0, 0,1,0,0 }, //M
			0x3388, // { 0,0,1,1, 0,0,1,1, 1,0,0,0, 1,0,0,0 }, //N
			0xff00, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //O
			0xe311, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 0,0,0,1 }, //P
			0xff08, // { 1,1,1,1, 1,1,1,1, 0,0,0,0, 1,0,0,0 }, //Q
			0xe319, // { 1,1,1,0, 0,0,1,1, 0,0,0,1, 1,0,0,1 }, //R
			0xee88, // { 1,1,1,0, 1,1,1,0, 1,0,0,0, 1,0,0,0 }, //S
			0xc044, // { 1,1,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //T
			0x3f00, // { 0,0,1,1, 1,1,1,1, 0,0,0,0, 0,0,0,0 }, //U
			0x0322, // { 0,0,0,0, 0,0,1,1, 0,0,1,0, 0,0,1,0 }, //V
			0x334a, // { 0,0,1,1, 0,0,1,1, 0,1,0,0, 1,0,1,0 }, //W
			0x00aa, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 1,0,1,0 }, //X
			0x00a4, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 0,1,0,0 }, //Y
			0xcc22, // { 1,1,0,0, 1,1,0,0, 0,0,1,0, 0,0,1,0 }, //Z
			0x4844, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,0 }, //[
			0x0088, // { 0,0,0,0, 0,0,0,0, 1,0,0,0, 1,0,0,0 }, //\/
			0x8444, // { 1,0,0,0, 0,1,0,0, 0,1,0,0, 0,1,0,0 }, //]
			0x2020, // { 0,0,1,0, 0,0,0,0, 0,0,1,0, 0,0,0,0 }, //^
			0x0c00, // { 0,0,0,0, 1,1,0,0, 0,0,0,0, 0,0,0,0 }, //_
			0x0080, // { 0,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,0,0 }, //`
			//
			0x0c07, // { 0,0,0,0, 1,1,0,0, 0,0,0,0, 0,1,1,1 }, //a
			0x0705, // { 0,0,0,0, 0,1,1,1, 0,0,0,0, 0,1,0,1 }, //b
			0x0601, // { 0,0,0,0, 0,1,1,0, 0,0,0,0, 0,0,0,1 }, //c
			0x0645, // { 0,0,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,1 }, //d
			0x0603, // { 0,0,0,0, 0,1,1,0, 0,0,0,0, 0,0,1,1 }, //e
			0x8301, // { 1,0,0,0, 0,0,1,1, 0,0,0,0, 0,0,0,1 }, //f
			0x8545, // { 1,0,0,0, 0,1,0,1, 0,1,0,0, 0,1,0,1 }, //g
			0x0305, // { 0,0,0,0, 0,0,1,1, 0,0,0,0, 0,1,0,1 }, //h
			0x8c05, // { 1,0,0,0, 1,1,0,0, 0,0,0,0, 0,1,0,1 }, //i
			0x0644, // { 0,0,0,0, 0,1,1,0, 0,1,0,0, 0,1,0,0 }, //j
			0x006c, // { 0,0,0,0, 0,0,0,0, 0,1,1,0, 1,1,0,0 }, //k
			0x8844, // { 1,0,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,0 }, //l
			0x1215, // { 0,0,0,1, 0,0,1,0, 0,0,0,1, 0,1,0,1 }, //m
			0x0205, // { 0,0,0,0, 0,0,1,0, 0,0,0,0, 0,1,0,1 }, //n
			0x0605, // { 0,0,0,0, 0,1,1,0, 0,0,0,0, 0,1,0,1 }, //o
			0x8341, // { 1,0,0,0, 0,0,1,1, 0,1,0,0, 0,0,0,1 }, //p
			0x8945, // { 1,0,0,0, 1,0,0,1, 0,1,0,0, 0,1,0,1 }, //q
			0x0014, // { 0,0,0,0, 0,0,0,0, 0,0,0,1, 0,1,0,0 }, //r
			0x0818, // { 0,0,0,0, 1,0,0,0, 0,0,0,1, 1,0,0,0 }, //s
			0x0855, // { 0,0,0,0, 1,0,0,0, 0,1,0,1, 0,1,0,1 }, //t
			0x0e04, // { 0,0,0,0, 1,1,1,0, 0,0,0,0, 0,1,0,0 }, //u
			0x0202, // { 0,0,0,0, 0,0,1,0, 0,0,0,0, 0,0,1,0 }, //v
			0x1e04, // { 0,0,0,1, 1,1,1,0, 0,0,0,0, 0,1,0,0 }, //w
			0x0066, // { 0,0,0,0, 0,0,0,0, 0,1,1,0, 0,1,1,0 }, //x
			0x00a2, // { 0,0,0,0, 0,0,0,0, 1,0,1,0, 0,0,1,0 }, //y
			0x0403, // { 0,0,0,0, 0,1,0,0, 0,0,0,0, 0,0,1,1 }, //z
			// a-z
			0x4845, // { 0,1,0,0, 1,0,0,0, 0,1,0,0, 0,1,0,1 }, //{
			0x0044, // { 0,0,0,0, 0,0,0,0, 0,1,0,0, 0,1,0,0 }, //|
			0x8454, // { 1,0,0,0, 0,1,0,0, 0,1,0,1, 0,1,0,0 }, //}
			0xa150, // { 1,0,1,0, 0,0,0,1, 0,1,0,1, 0,0,0,0 }, //~
			0x3fa0  // { 0,0,0,0, 1,1,0,0, 0,0,0,0, 1,0,1,0 }, //DEL
		);
		layout (points) in;
		layout (triangle_strip, max_vertices=100) out;
		uniform mat4 MVP;

		in vec3 vertColor[];
		in int character[];
		out vec3 fragColor;

		void main() {
			vec4 scaleVector = vec4(0.04, 0.04, 1.0, 1.0);

			fragColor = vec3(0.0, 0.10, 0.15);
			gl_Position = MVP * ( gl_in[0].gl_Position + vec4( -12.5, -12.5, 0.0, 0.0 ) * scaleVector ); EmitVertex();
			gl_Position = MVP * ( gl_in[0].gl_Position + vec4(  12.5, -12.5, 0.0, 0.0 ) * scaleVector ); EmitVertex();
			gl_Position = MVP * ( gl_in[0].gl_Position + vec4( -12.5,  12.5, 0.0, 0.0 ) * scaleVector ); EmitVertex();
			gl_Position = MVP * ( gl_in[0].gl_Position + vec4(  12.5,  12.5, 0.0, 0.0 ) * scaleVector ); EmitVertex();
			EndPrimitive();


			int ch = led16_ee[character[0]];
			int s = 0x8000;
			for(int seg=0; seg < 192; seg+=12){
				if( (s & ch) == 0 ){
					fragColor = vec3( 0.08, 0.08, 0.08 );
				} else {
					fragColor = vertColor[0];
				}
				for(int v=0; v < 12; v+=2){
					gl_Position = MVP * (gl_in[0].gl_Position + vec4( led16_vertex_data[seg+v], led16_vertex_data[seg+v+1], 0, 0) * scaleVector);
					EmitVertex();
				}
				EndPrimitive();
				s >>= 1;
			}
		}
	);
	led.fragment.source = GLSL(
		in vec3 fragColor;
		out vec4 outColor;
		void main() {
			outColor = vec4( fragColor, 1.0 );
		}
	);
	
	setupProgram(&led);

	/* Position attribute */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	/* Color attribute */
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	/* Character attribute */
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	led.verts.draw_size /= 7;
}


void setupBcr(){
	static GLfloat projectionMatrix[16] = {
		0.5,0,0,-0.95,
		0,0.5,0,-0.65,
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

int main(void) {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GL Keys", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, WIDTH, HEIGHT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setupAdsr();
	setupPiano();
	setupLed();
	setupBcr();

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		//RUN_PROGRAM(adsr);
		runProgram(&adsr);
		runProgram(&piano);
		runProgram(&led);
		runProgram(&bcr);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &(adsr.verts.vao));
	glDeleteBuffers(1, &(adsr.verts.vbo));
	glfwTerminate();
	return EXIT_SUCCESS;
}
