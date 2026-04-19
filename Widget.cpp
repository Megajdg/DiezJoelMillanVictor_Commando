#include "Widget.h"
#include "Scene.h"
#include "GraphicsInterface.h"

Widget::Widget(Scene* myScene) :Actor(myScene)
{
}

void Widget::Update(float deltaTime)
{
}

// Dibuja texto en pantalla usando las coordenadas de la pantalla no del mundo
void Widget::Render()
{
	myScene->GI->PrintText(
		"pepito",			// Texto
		10, 10,				// Posicion en pantalla
		{ 255, 0, 0 },		// Color rgb
		EFONT_SIZE::BIG);	// Tamaño de fuente
}