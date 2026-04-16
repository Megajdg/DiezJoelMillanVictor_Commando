#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include "Transform.h"
#include "SDL3/SDL.h"

struct bColor
{
	unsigned char R, G, B, A;
};

struct fColor
{
	float r, g, b, a;
};

enum class EFONT_SIZE
{
	SMALL, MEDIUM, BIG
};

struct MapInfo
{
	int realWidth;
	int realHeight;
	float scaleX;
	float scaleY;
};

class GraphicsInterface
{
	class SDL_Window* window;
	class SDL_Renderer* renderer;

	std::map<std::string, class SDL_Texture*> image_collection;

	struct Glyph
	{
		class SDL_Texture* glyphTexture;
		float width, height;
	};
	std::map<char, Glyph*> glyphs;
	std::map<char, Glyph*> grenadeGlyphs;
	class TTF_Font* font = NULL;
	class TTF_Font* grenadeFont = NULL;

	void InitFontAndGlyphs();
	void DestroyFontAndGlyphs();

public:

	GraphicsInterface();
	~GraphicsInterface();

	bool LoadImage(std::string path);

	bool MustWindowClose();

	void DrawSprite(std::string img_name, Transform transform, Vector2 size);

	void DrawSprite(std::string img_name, Transform transform, Vector2 size, SDL_FRect* srcRect);

	void DrawFrame();

	void DrawRectScreen(float x, float y, float w, float h, unsigned char r, unsigned char g, unsigned char b);

	void ClearScreen(unsigned char r, unsigned char g, unsigned char b);

	std::vector<uint8_t> LoadCollisionMask(const std::string& path, int& width, int& height);

	MapInfo GetMapInfo(const std::string& path, float renderedWidth, float renderedHeight);

	int PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize);
	int PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize, bool grenade);
};

