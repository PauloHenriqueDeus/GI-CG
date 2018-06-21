#pragma once
#define WIN32_LEAN_AND_MEAN		

#include <stdio.h>
#include <stdlib.h>
#include <glut.h>
#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <iostream>
#include <string>

#include <ctime>

#include "Renderer.h"
#include "Transform.h"
#include "Ray.h"
#include "Sphere.h"
#include "wtypes.h"
#include "Light.h"
#include "Cube.h"
#include "Scene.h"

int windowW = 1024;
int windowH = 720;

Renderer* renderer;

double lastTime = 0;

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void ElapsedTime() {
	std::string s = "Global Illumintation | Elapsed:" + std::to_string((((float)(glutGet(GLUT_ELAPSED_TIME) - lastTime)/1000.0f)));
	glutSetWindowTitle(s.c_str());
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	Image* display = renderer->GetDisplay();
	glDrawPixels(display->getWidth(), display->getHeight(), GL_BGRA_EXT, GL_UNSIGNED_BYTE,
		display->getPixels());

	glFinish();
	glutSwapBuffers();
}

void Idle() {

}

bool InitGL() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);
	renderer->Initialize();

	return true;
}

int main(int argc, char** argv) {

	renderer = new Renderer(windowW, windowH);
	Scene::LoadSceneList("sceneList");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowW, windowH);

	int SCREEN_H = 0;
	int SCREEN_W = 0;
	GetDesktopResolution(SCREEN_W, SCREEN_H);

	glutInitWindowPosition((int)(SCREEN_W / 2 - windowW / 2), (int)(SCREEN_H / 2 - windowH / 2));
	glutCreateWindow("Global Illumination");


	if (!InitGL()) { return 1; }

	do {
		lastTime = glutGet(GLUT_ELAPSED_TIME);
		renderer->Render();
		ElapsedTime();
	} while (Scene::NextScene());
	
	Scene::UnloadScene();

	//glutFullScreen();

	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	glutMainLoop();

	return 0;

}