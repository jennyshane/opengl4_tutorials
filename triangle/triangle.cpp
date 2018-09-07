#include <GL/glew.h>
#include <SDL/SDL.h>
#include <GL/glu.h>
#include <iostream>

struct glPointStr{
	GLfloat x, y, z, w;
	GLfloat r, g, b, a;
	glPointStr(float X, float Y, float Z, float R, float G, float B):x(X), y(Y), z(Z), r(R), g(G), b(B)
	{
		w=1.0f;
		a=1.0f;
	}
	glPointStr(){
		x=0.0f;
		y=0.0f;
		z=0.0f;
		w=1.0f;
		r=0.0f;
		g=0.0f;
		b=0.0f;
		a=1.0f;
	}
};

int screensize=400;

int main(int argc, char** argv){
	const SDL_VideoInfo* info=NULL;
	if (-1==SDL_Init(SDL_INIT_EVERYTHING)){
		std::cout<<"Couldn't init sdl."<<std::endl;
		return 1;
	}

	info=SDL_GetVideoInfo();
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	if(NULL==SDL_SetVideoMode(screensize, screensize, info->vfmt->BitsPerPixel, SDL_OPENGL)){
		std::cout<<"Couldn't set sdl parameters."<<std::endl;
		return 1;
	}
	glewInit();
	SDL_WM_SetCaption("OpenGL Window", NULL);
	SDL_EnableUNICODE(1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glPointStr points[]={glPointStr(0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f), glPointStr(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f), 
		glPointStr(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f)};
	
	//{0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f};
	//define shaders as char arrays in program memory
	const char *vshader="#version 460\n"
		"in vec3 vertex_position;"
		"in vec3 vertex_color;"
		"out vec3 color;"
		"void main(){"
		"gl_Position=vec4(vertex_position, 1.0);"
		"color=vertex_color;"
		"}";
	const char *fshader="#version 460\n"
		"in vec3 color;"
		"out vec4 frag_color;"
		"void main(){"
		"frag_color=vec4(color, 1.0);"
		"}";
	GLuint vert_shader, frag_shader;
	GLuint shader_program;
	//compile vertex shader from char array source
	vert_shader=glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert_shader, 1, &vshader, NULL);
	glCompileShader(vert_shader);
	//compile fragment shader from char array source
	frag_shader=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fshader, NULL);
	glCompileShader(frag_shader);
	//create shader program from vertex and fragment shaders
	shader_program=glCreateProgram();
	glAttachShader(shader_program, frag_shader);
	glAttachShader(shader_program, vert_shader);
	//bind attributes to fixed location-- doing this instead of using the layout
	//keyword in the shader code maintains compatibility with opengl3
	glBindAttribLocation(shader_program, 0, "vertex_position"); 
	glBindAttribLocation(shader_program, 1, "vertex_color");
	glLinkProgram(shader_program);

	GLuint vao;
	GLuint vbo;

	//create vertex buffer object to hold point data
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(glPointStr), points, GL_STATIC_DRAW);
	//Vertex arrays store data about how to read vertex data from the buffers. 
	//Enabling a vertex array will load all previously set configurations. 
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); //only have to bind one vbo since the position and color data are packed together. 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glPointStr), (void*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glPointStr), (void*)(offsetof(glPointStr, r)));
	SDL_Event event;

	bool quit=false;
	while(false==quit){
		while(SDL_PollEvent(&event)){
			if(SDL_QUIT==event.type){
				quit=true;
			}		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		SDL_GL_SwapBuffers();
	}

	SDL_Quit();

	return 0;
}
