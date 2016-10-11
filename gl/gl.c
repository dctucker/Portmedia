#include "common.h"

_program adsr, piano;


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

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		//RUN_PROGRAM(adsr);
		runProgram(&adsr);
		runProgram(&piano);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &(adsr.verts.vao));
	glDeleteBuffers(1, &(adsr.verts.vbo));
	glfwTerminate();
	return EXIT_SUCCESS;
}
