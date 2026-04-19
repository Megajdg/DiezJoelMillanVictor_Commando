#pragma once
#include "Vector2.h"

/// <summary>
/// Contiene constantes y valores confiigurables del juego
/// </summary>
struct Parameters
{
	static const char* window_title;				// Titulo de la ventana del juego
	static const int width;							// Ancho del juego
	static const int height;						// Alto del juego
	static const int desired_FPS;					// FPS objetivo del motor
	static const Vector2 initial_camera_position;	// Posicion inicial de la camara en el mundo
	static const float bigFontSize;					// Tamaño de fuente grande
	static const float mediumFontSize;				// Tamaño de fuente medio
	static const float smallFontSize;				// Tamaño de fuente pequeño
	static int screenWidth;							// Ancho real de pantalla
	static int screenHeight;						// Alto real de pantalla
};