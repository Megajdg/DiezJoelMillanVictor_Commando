#include "Parameters.h"

const char* Parameters::window_title = "DiezJoelMillanVictor_Commando\0";	// Titulo de la ventada del juego
const int Parameters::width = 1920;											// Ancho de la pantalla base
const int Parameters::height = 1080;										// Alto de la pantalla base
const int Parameters::desired_FPS = 60;										// FPS objetivo del motor
const Vector2 Parameters::initial_camera_position(0, 0);					// Posicion inicial de la camara en el mundo
const float Parameters::bigFontSize = 40.f;									// Tamaño de fuente grande
const float Parameters::mediumFontSize = 25.f;								// Tamaño de fuente medio
const float Parameters::smallFontSize = 10.f;								// Tamaño de fuente pequeño
int Parameters::screenWidth = Parameters::width;							// Ancho real de la pantalla
int Parameters::screenHeight = Parameters::height;							// Alto real de la pantalla
