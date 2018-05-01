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

bool InitGL() {

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearDepth(1.0f);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	renderer->Initialize();

	return true;
}

int main(int argc, char** argv) {

	renderer = new Renderer(windowW, windowH);
	Sphere* sphere  = new Sphere(Vector3( 0, 0, -50), 5, Material::LoadMaterial("material"));
	Sphere* sphere2 = new Sphere(Vector3(-8, 0, -70), 5, Material::LoadMaterial("material2"));

	AACube* p = new AACube(Vector3(25, 0, -50), Vector3(50, 50, 1), Material::GetMaterial("material2"));
	//AACube* p2 = new AACube(Vector3(-25, 0, -50), Vector3(50, 50, 1), Material::GetMaterial("material2"));
	//AACube* p3 = new AACube(Vector3(0, 0, -50-25), Vector3(50, 50, 1), Material::GetMaterial("material2"));

	p->GetTransform()->Rotate(Vector3(0, -90, 0));
	//p2->GetTransform()->Rotate(Vector3(0, 90, 0));

	Light light = Light(Vector3(8, 0, 0), 10);
	//Light light2 = Light(Vector3(-8, 0, -80), 10);

	/*windowW = width;
	windowH = height;*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowW, windowH);

	int SCREEN_H = 0;
	int SCREEN_W = 0;
	GetDesktopResolution(SCREEN_W, SCREEN_H);

	glutInitWindowPosition((int)(SCREEN_W / 2 - windowW / 2), (int)(SCREEN_H / 2 - windowH / 2));
	glutCreateWindow("Global Illumination");


	if (!InitGL()) { return 1; }

	/*Para cada pixel da imagem
	Calcular raio que passa pelo pixel e pelo olho
	Determinar objeto atingido pelo raio
	Ponto de intersecção
	Normal
	Propriedades de material
	Propriedades de textura
	Computar contribuição da iluminação ambiente
	Para cada fonte de luz, determinar a visibilidade(raio de
	sombra)
	Se fonte visível, somar contribuição da reflexão difusa
	Se o limite de recursão não foi atingido
	Somar contribuição da reflexão especular
	acompanhando raio refletido
	Somar contribuição de transmissão acompanhando raio
	refratado*/
	lastTime = glutGet(GLUT_ELAPSED_TIME);


	//RayTracing();
	renderer->Render();


	ElapsedTime();

	//glutFullScreen();

	glutDisplayFunc(Display);
	glutIdleFunc(Display);

	glutMainLoop();

	return 0;

}