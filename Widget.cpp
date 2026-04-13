#include "Widget.h"
#include "Scene.h"
#include "GraphicsInterface.h"

Widget::Widget(Scene* myScene):Actor(myScene)
{
}

void Widget::Update(float deltaTime)
{
}

void Widget::Render()
{
	myScene->GI->PrintText("pepito", 10, 10, { 255, 0, 0 }, EFONT_SIZE::BIG);
}
