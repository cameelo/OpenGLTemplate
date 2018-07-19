#include <iostream>
#include <ctime>
#include <windows.h>
#include "Utils.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"

using namespace std;

void init();
void initGL();
void draw();
void close();

SDL_Window* window = NULL;
SDL_GLContext context;

int WIDTH = 800;
int HEIGHT = 600;

void init() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	context = SDL_GL_CreateContext(window);
	glewExperimental = GL_TRUE;
	glewInit();
	initGL();
}

void initGL() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//OpenGL attribs
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void close() {
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_Quit();
}



int main(int argc, char* argv[]) {
	init();
	Camera cam = Camera(WIDTH, HEIGHT, 45, window);
	Mesh* water = new Mesh();
	ShaderProgram* pass = new ShaderProgram("assets/shaders/pass.vert","assets/shaders/pass.frag");
	bool exit = false;

	bool wireframe = false;

	SDL_Event event;
	
	double frameTime = 1000.0f / 65.0f;

	std::clock_t start;
	while (!exit) {
		start = clock();
		while (SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				exit = true;
				break;
			case SDL_KEYDOWN: {
				if (event.key.keysym.sym == SDLK_ESCAPE) {
					exit = true;
					break;
				}
				else if (event.key.keysym.sym == SDLK_m) {
					cam.moveCam();
					break;
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN: {
			}
			break;
			}
		}
		cam.update();
		draw();
		pass->bind();
		GLuint worldTransformID = glGetUniformLocation(pass->getId(), "worldTransform");
		glUniformMatrix4fv(worldTransformID, 1, GL_FALSE, &cam.modelViewProjectionMatrix[0][0]);
		GLuint vistaID = glGetUniformLocation(pass->getId(), "vista");
		glUniform3fv(vistaID,1,&((cam.ref-cam.pos)[0]));
		water->draw();
		pass->unbind();
		double dif = frameTime - ((clock() - start) * (1000.0 / double(CLOCKS_PER_SEC)) );
		if (dif > 0) {
			Sleep(int(dif));
		}
		SDL_GL_SwapWindow(window);
	}
	close();
	return 0;
}
