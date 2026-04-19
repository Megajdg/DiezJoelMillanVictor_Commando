#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Transform.h"
#include "SDL3/SDL.h"

/// <summary>
/// Define color para textos y HUD
/// </summary>
struct bColor
{
	unsigned char R, G, B, A;
};

/// <summary>
/// Define los tamaños de fuente
/// </summary>
enum class EFONT_SIZE
{
	SMALL, MEDIUM, BIG
};

/// <summary>
/// Define informacion relevante de los mapas
/// </summary>
struct MapInfo
{
	int realWidth;
	int realHeight;
	float scaleX;
	float scaleY;
};

/// <summary>
/// Define la informacion esencial de los glifos de una fuente
/// </summary>
struct Glyph
{
	class SDL_Texture* glyphTexture;
	float width, height;
};

/// <summary>
/// Interfaz grafica
/// </summary>
class GraphicsInterface
{
	class SDL_Window* window;																					// Ventana SDL
	class SDL_Renderer* renderer;																				// Renderer SDL

	std::map<std::string, class SDL_Texture*> image_collection;													// Vector de texturas del juego

	std::map<char, Glyph*> glyphs;																				// Glifos de la fuente principal
	std::map<char, Glyph*> grenadeGlyphs;																		// Glifos de la fuente de la granada
	class TTF_Font* font = NULL;																				// Puntero a la fuente principal
	class TTF_Font* grenadeFont = NULL;																			// Puntero a la fuente de la granada

	void InitFontAndGlyphs();																					// Inicializador de fuentes y glifos
	void DestroyFontAndGlyphs();																				// Destructor de fuentes y glifos

public:
	GraphicsInterface();																						// Constructor
	~GraphicsInterface();																						// Destructor

	bool LoadImage(std::string path);																			// Carga una imagen en memoria
	void DrawSprite(std::string img_name, Transform transform, Vector2 size);									// Pinta un sprite en pantalla
	void DrawSprite(std::string img_name, Transform transform, Vector2 size, SDL_FRect* srcRect);				// Pinta un trozo de una spritesheet en pantalla
	void DrawFrame();																							// Renderiza un frame
	void DrawRectScreen(float x, float y, float w, float h, unsigned char r, unsigned char g, unsigned char b);	// Pinta un rectangulo negro
	void ClearScreen(unsigned char r, unsigned char g, unsigned char b);										// Limpieza de pantalla

	std::vector<uint8_t> LoadCollisionMask(const std::string& path, int& width, int& height);					// Decide donde ira colision en los mapas
	MapInfo GetMapInfo(const std::string& path, float renderedWidth, float renderedHeight);						// Calcula la escala real de los mapas

	int PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize);							// Pinta texto con la fuente normal
	int PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize, bool grenade);				// Pinta texto con la fuente de la granada

	Vector2 MeasureText(const std::string& text, EFONT_SIZE size);												// Mide los textos que recibe para poder centrarlos
};