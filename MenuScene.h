#pragma once
#include "scene.h"

/// <summary>
/// Escena de menu
/// </summary>
class MenuScene : public Scene
{
public:
	MenuScene(GraphicsInterface* GI, MyPhysics* mph);	// Constructor

	void Update(float dt) override;						// Gestiona el movimiento en el menu y la seleccion de opciones
	void Render() override;								// Pantalla negra, texto y logo

private:
	int selected = 0;									// Opcion del menu seleccionada: 0 -> Jugar, 1 -> Ayuda, 2 -> Creditos, 3 -> Salir
};