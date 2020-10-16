#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

//include shape, shader header files
#include "Triangle.h"
#include "ShaderClass.h"

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#ifndef GLEW_H
#define GLEW_H
//#include <GL/glew.h>
#include "windows.h"

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
//#include "SDL_image.h"
//#include "SDL_mixer.h"
//#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



int main(int argc, char *argv[]) {

	high_resolution_clock::time_point t1 = high_resolution_clock::now();

	//SDL Initialise
	SDL_Init(SDL_INIT_EVERYTHING);

	//SDL create window
	SDL_Window *win = SDL_CreateWindow("CGP3018M OpenGL Window", 100, 100, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	//set context attributes
	//sets opengl version to 4.3
	int major = 4, minor = 3;

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE); //use core profile
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); //ask for forward compatible
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(win);

	//GLEW initialise
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	//*****************************************************
	//OpenGL specific data
	//create objects
	//define vertices for the triangle
	GLfloat vertices[9] = {
		//-0.5f, -0.5f, 0.0f,	
		//-0.5f,  0.5f, 0.0f,	
		//0.0f, 0.0f, 0.0f
		0.5f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.0f, 0.0f

	};

	GLfloat vertices2[9] = {
		-0.9f, -0.9f, 0.0f,
		-0.9f,  0.9f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	Triangle tri(vertices);
	Triangle tri2(vertices2);

	//create shaders
	Shader vSh("..//..//Assets//Shaders//shader.vert");
	Shader fSh("..//..//Assets//Shaders//shader.frag");

	//create, allocate and compile shaders
	//compile the shader code
	//1 for vertex, 2 for fragment - there is probably a better way to do this
	vSh.getShader(1);
	fSh.getShader(2);

	//create shader program, attach shaders together in the shader program
	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vSh.shaderID);
	glAttachShader(shaderProgram, fSh.shaderID);
	glLinkProgram(shaderProgram);

	//delete shader source code pointers
	glDeleteShader(vSh.shaderID);
	glDeleteShader(fSh.shaderID);

	//OpenGL buffers
	//set buffers for the triangle
	tri.setBuffers();
	tri2.setBuffers();

	//***********************************************

	SDL_Event event;
	bool windowOpen = true;
	float color[3] = { 0.0f, 0.0f, 0.0f };

	//*****************************
	//'game' loop
	while (windowOpen)
	{

		GLint loc = glGetUniformLocation(shaderProgram, "uTime");
		if (loc != -1) {
			high_resolution_clock::time_point t2 = high_resolution_clock::now();
			duration<float, std::milli> time_span = t2 - t1;
			glUniform1f(loc, time_span.count());
		}

		//****************************
		// OpenGL calls.

		glClearColor(color[0], color[1], color[2], 1);
		glClear(GL_COLOR_BUFFER_BIT); 

		//draw the triangles
		//Use shader program we have compiled and linked
		glUseProgram(shaderProgram);
		glBindVertexArray(tri.VAO);
		//set to wireframe so we can see the 2 triangles
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(tri2.VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		//unbind (release) the VAO
		glBindVertexArray(0);

		SDL_GL_SwapWindow(win);


		//*****************************
		//SDL handled input
		//Any input to the program is done here
		if (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				windowOpen = false;
			}
			if (event.type == SDL_KEYDOWN)
			{
				switch (event.key.keysym.sym) {

				case SDLK_c:
					windowOpen = false;
					break;

				case SDLK_1:
					color[0] = 1.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
					break;

				case SDLK_2:
					color[0] = 0.0f;
					color[1] = 1.0f;
					color[2] = 0.0f;
					break;

				case SDLK_3:
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 1.0f;
					break;

				case SDLK_4:
					color[0] = 0.0f;
					color[1] = 0.0f;
					color[2] = 0.0f;
					break;
				}
			}
		}
	}
	//***1*************************
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);

	SDL_Quit();
	return 0;





}
#endif