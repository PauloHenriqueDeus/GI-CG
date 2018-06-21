#include "Renderer.h"
#include "Scene.h"

Renderer* Renderer::renderer = nullptr;


Renderer::~Renderer()
{
}

void Renderer::ExportImage()
{
	display->ExportBMP(Scene::CurrentScene()->GetName());
	nprStroke->ExportBMP("nprStroke");
}
