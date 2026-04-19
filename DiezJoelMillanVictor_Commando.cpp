// DiezJoelMillanVictor_Commando.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include "Game.h"

int main()
{
	// Creamos el juego
	Game::Create();

	// Ponemos el juego en marcha
	Game::Play();

	// Destruimos el juego
	Game::Destroy();
}
