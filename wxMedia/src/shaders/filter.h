_program filter;

void setupFilter(){
	static GLfloat projectionMatrix[16] = {
		0.5,0,0,0.4,
		0,0.5,0,0.5,
		0,0,1,0,
		0,0,0,1
	};
	static GLfloat filter_db[] = {
		-36, -30, -24, -20, -18, -15, -12, -9,
		-8, -3, -2, 2, 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2, 6, 6,
		 7, 8, 7, 6, 6, 6, 5, 4,
		 3, 2, 2, 24, 2, 0, 0, 0,
		 0, 0, 0, -1, -1, -1, -1, -1,
		-2, -3, -4, -5, -10, -11, -16, -20,
		-30, -30, -30, -30, -24, -20, -18, -15,
		-12, -9, -8, -3, -2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2, 2, 2, 2, 2, 2, 2,
		 2, 2, 2, 2, 6, 6, 7, 7,
		 7, 6, 6, 6, 5, 4, 3, 2,
		 2, 2, 2, 0, 0, 0, 0, 0,
		 0, -1, -1, -1, -1, -1, -2, -3,
		-4, -5, -10, -11, -16, -20, -30, -44
	};

	filter.verts.data = filter_db;
	filter.verts.size = sizeof(filter_db);
	filter.MVP.data = projectionMatrix;
	filter.verts.usage = GL_DYNAMIC_DRAW;

	filter.vertex.source = GLSL(
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
	filter.geometry.source = GLSL(
		uniform float time;
		uniform mat4 MVP;
		layout (points) in;
		layout (triangle_strip, max_vertices=6) out;

		in float f0[];
		in float f1[];
		in int index[];
		out vec4 fragColor;

		void main(){
			float alpha = 0.4;
			int mindb = -36;
			int maxdb = 24;
			int x = 0; //-inst;
			int y = -mindb; //inst * 0.2f;
			int z = 1; //inst / 8.0f;
			vec3 scaleVector = vec3( 1.0/128.0, 1.0/(maxdb-mindb), 1 );
			
			fragColor = vec4(0.0, 1.0, 0.0, alpha * (1+0.5*sin(time*3.14)) );
			
			//glNormal3f( 0.0, 0.0, 1.0 );

			int i = index[0];

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
	filter.fragment.source = GLSL(
		in vec4 fragColor;
		out vec4 outColor;
		void main() {
			outColor = fragColor;
		}
	);

	setupProgram(&filter);
	
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(GLfloat), (GLvoid*)(1 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	filter.verts.draw_size -= sizeof(GLfloat);
}
