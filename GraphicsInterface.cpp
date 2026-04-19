#include "GraphicsInterface.h"
#include "Parameters.h"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "Game.h"

GraphicsInterface::GraphicsInterface()
{
	//Abrimos ventana e inicializamos gráficos
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(Parameters::window_title, Parameters::width, Parameters::height, SDL_WINDOW_FULLSCREEN, &window, &renderer);

	// Calculamos tamaño de pantalla real, para que todo funcione en cualquier resolucion
	int realW, realH;
	SDL_GetWindowSize(window, &realW, &realH);

	Parameters::screenWidth = realW;
	Parameters::screenHeight = realH;

	// Inicializamos fuentes
	InitFontAndGlyphs();
}

void GraphicsInterface::InitFontAndGlyphs()
{
	// Inicializamos SDL_ttf
	TTF_Init();

	// Cargamos una fuente
	font = TTF_OpenFont("Commodore-64-v6.3.ttf", Parameters::bigFontSize);

	// Precargamos todos los glifos y los guardamos
	SDL_Color color = { 255, 255, 255 };
	for (char c = '('; c <= 'z'; c++)
	{
		SDL_Surface* srfc = TTF_RenderGlyph_Blended(font, c, color);
		if (srfc)
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, srfc);
			float texW = 0;
			float texH = 0;
			SDL_GetTextureSize(texture, &texW, &texH);
			glyphs.insert({ c, new Glyph{texture, texW, texH} });
			SDL_DestroySurface(srfc);
		}
	}

	// Cargamos otra fuente (solo para el icono de granada del hud)
	grenadeFont = TTF_OpenFont("WolveSymbol.ttf", Parameters::bigFontSize);

	// Precargamos todos los glifos y los guardamos (no es óptimo porque solo queremos uno, pero bueno)
	color = { 255, 255, 255 };
	for (char c = '('; c <= 'z'; c++)
	{
		SDL_Surface* srfc = TTF_RenderGlyph_Blended(grenadeFont, c, color);
		if (srfc)
		{
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, srfc);
			float texW = 0;
			float texH = 0;
			SDL_GetTextureSize(texture, &texW, &texH);
			grenadeGlyphs.insert({ c, new Glyph{texture, texW, texH} });
			SDL_DestroySurface(srfc);
		}
	}
}

void GraphicsInterface::DestroyFontAndGlyphs()
{
	// Recorremos todos los glifos y los destruimos
	for (auto pair : glyphs)
	{
		SDL_DestroyTexture(pair.second->glyphTexture);
		delete pair.second;
	}

	// Recorremos todos los glifos y los destruimos
	for (auto pair : grenadeGlyphs)
	{
		SDL_DestroyTexture(pair.second->glyphTexture);
		delete pair.second;
	}

	// Cerramos las dos fuentes y el SDL_ttf
	TTF_CloseFont(font);
	TTF_CloseFont(grenadeFont);
	TTF_Quit();
}

int GraphicsInterface::PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize)
{
	// Seleccionamos tamaño de fuente
	int font_size{};
	switch (fontsize)
	{
	case EFONT_SIZE::BIG:
		font_size = Parameters::bigFontSize;
		break;
	case EFONT_SIZE::MEDIUM:
		font_size = Parameters::mediumFontSize;
		break;
	case EFONT_SIZE::SMALL:
		font_size = Parameters::smallFontSize;
		break;
	default:
		font_size = Parameters::bigFontSize;
		break;
	}
	float fontScale = (1.f * font_size) / Parameters::bigFontSize;

	// Recorremos cada caracter, aplica el glifo correspondiente con el color deseado, renderiza la textura y avanza la posicion X segun el ancho real
	SDL_FRect dstrect = { x, y, 0, 0 };
	for (int i = 0; i < text.size(); i++)
	{
		char c = text.at(i);
		auto it = glyphs.find(c);
		if (it != glyphs.end())
		{
			Glyph* g = glyphs.at(c);
			SDL_SetTextureColorMod(g->glyphTexture, color.R, color.G, color.B);
			dstrect.w = g->width * fontScale;
			dstrect.h = g->height * fontScale;
			SDL_RenderTexture(renderer, g->glyphTexture, NULL, &dstrect);
			dstrect.x += std::min((int)(g->width * fontScale), font_size);
		}
		else
			dstrect.x += font_size;
	}
	return dstrect.x;
}

// Este es para pintar la granada en el HUD
int GraphicsInterface::PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize, bool grenade)
{
	// Seleccionamos tamaño de fuente
	int font_size{};
	switch (fontsize)
	{
	case EFONT_SIZE::BIG:
		font_size = Parameters::bigFontSize;
		break;
	case EFONT_SIZE::MEDIUM:
		font_size = Parameters::mediumFontSize;
		break;
	case EFONT_SIZE::SMALL:
		font_size = Parameters::smallFontSize;
		break;
	default:
		font_size = Parameters::bigFontSize;
		break;
	}
	float fontScale = (1.f * font_size) / Parameters::bigFontSize;

	// Recorremos cada caracter, aplica el glifo correspondiente con el color deseado, renderiza la textura y avanza la posicion X segun el ancho real
	SDL_FRect dstrect = { x, y, 0, 0 };
	for (int i = 0; i < text.size(); i++)
	{
		char c = text.at(i);
		auto it = grenadeGlyphs.find(c);
		if (it != grenadeGlyphs.end())
		{
			Glyph* g = grenadeGlyphs.at(c);
			SDL_SetTextureColorMod(g->glyphTexture, color.R, color.G, color.B);
			dstrect.w = g->width * fontScale;
			dstrect.h = g->height * fontScale;
			SDL_RenderTexture(renderer, g->glyphTexture, NULL, &dstrect);
			dstrect.x += std::min((int)(g->width * fontScale), font_size);
		}
		else
			dstrect.x += font_size;
	}
	return dstrect.x;
}

Vector2 GraphicsInterface::MeasureText(const std::string& text, EFONT_SIZE fontsize)
{
	// Seleccionamos tamaño de fuente
	int font_size{};
	switch (fontsize)
	{
	case EFONT_SIZE::BIG:
		font_size = Parameters::bigFontSize;
		break;
	case EFONT_SIZE::MEDIUM:
		font_size = Parameters::mediumFontSize;
		break;
	case EFONT_SIZE::SMALL:
		font_size = Parameters::smallFontSize;
		break;
	default:
		font_size = Parameters::bigFontSize;
		break;
	}
	float fontScale = (1.f * font_size) / Parameters::bigFontSize;

	float x = 0.0f;
	float maxH = 0.0f;

	// Calculamos ancho real y altura maxima
	for (int i = 0; i < (int)text.size(); i++)
	{
		char c = text.at(i);
		auto it = glyphs.find(c);
		if (it != glyphs.end())
		{
			Glyph* g = it->second;
			float w = g->width * fontScale;
			float h = g->height * fontScale;

			x += std::min((int)w, font_size);
			if (h > maxH) maxH = h;
		}
		else
		{
			x += font_size;
		}
	}

	// Devolvemos lo que ocupa el texto indicado
	return Vector2(x, maxH);
}

GraphicsInterface::~GraphicsInterface()
{
	// Destruimos todas las texturas
	for (auto pair : image_collection)
		SDL_DestroyTexture(pair.second);

	// Destruimos fuentes, el renderer, la ventana y cerramos SDL
	DestroyFontAndGlyphs();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool GraphicsInterface::LoadImage(std::string path)
{
	// Cargamos textura y la insertamos en el vector correspondiente
	SDL_Texture *txt= IMG_LoadTexture(renderer, path.c_str());
	image_collection.insert(std::make_pair(path, txt));
	return false;
}

void GraphicsInterface::DrawSprite(std::string img_name, Transform transform, Vector2 size)
{
	// Pintamos un sprite, lo centramos teniendo en cuenta la posicion de la camara y añadiendo el offset de pantalla
	SDL_FRect rect;
	rect.w = size.x*transform.scale.x;
	rect.h = size.y*transform.scale.y;
	rect.x = transform.position.x - rect.w * 0.5f - Game::camera.position.x + Parameters::screenWidth * 0.5f;
	rect.y = transform.position.y - rect.h * 0.5f - Game::camera.position.y + Parameters::screenHeight * 0.5f;

	SDL_RenderTextureRotated(renderer, image_collection[img_name],
		NULL, &rect,
		transform.rotation, NULL,
		SDL_FlipMode::SDL_FLIP_NONE);

}

void GraphicsInterface::DrawSprite(std::string img_name, Transform transform, Vector2 size, SDL_FRect* srcRect)
{
	// Pintamos una parte de la spritesheet, la centramos teniendo en cuenta la posicion de la camara y añadiendo el offset de pantalla
	SDL_FRect rect;
	rect.w = size.x * transform.scale.x;
	rect.h = size.y * transform.scale.y;
	rect.x = transform.position.x - rect.w * 0.5f - Game::camera.position.x + Parameters::screenWidth * 0.5f;
	rect.y = transform.position.y - rect.h * 0.5f - Game::camera.position.y + Parameters::screenHeight * 0.5f;

	SDL_RenderTexture(renderer, image_collection[img_name], srcRect, &rect);
}

void GraphicsInterface::ClearScreen(unsigned char r, unsigned char g, unsigned char b)
{
	// Limpiamos la pantalla
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderClear(renderer);
}

void GraphicsInterface::DrawFrame()
{
	// Renderiza un frame
	SDL_RenderPresent(renderer);
}

void GraphicsInterface::DrawRectScreen(float x, float y, float w, float h, unsigned char r, unsigned char g, unsigned char b)
{
	// Dibuja un rectangulo negro (se usa para el HUD)
	SDL_FRect rect = { x, y, w, h };
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

std::vector<uint8_t> GraphicsInterface::LoadCollisionMask(const std::string& path, int& width, int& height)
{
	// Cargamos el png como surface (ya que es mas rapido que como textura)
	SDL_Surface* surf = IMG_Load(path.c_str());
	if (!surf) return {};

	int w = surf->w;
	int h = surf->h;
	width = w;
	height = h;
	std::vector<uint8_t> mask(w * h);

	const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(surf->format);
	const SDL_Palette* pal = nullptr;

	// Leemos los pixeles del png
	Uint32* pixels = (Uint32*)surf->pixels;

	// Extraemos de cada pixel su color y aplicamos la regla de colision (si tiene negro es 1, si tiene blanco es 0)
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			Uint32 pixel = pixels[y * w + x];

			Uint8 r, g, b;
			SDL_GetRGB(pixel, fmt, pal, &r, &g, &b);

			mask[y * w + x] = (r < 128 && g < 128 && b < 128) ? 1 : 0;
		}
	}

	// Destruimos la surface
	SDL_DestroySurface(surf);
	return mask;
}

MapInfo GraphicsInterface::GetMapInfo(const std::string& path, float renderedWidth, float renderedHeight)
{
	// Cargamos el mapa en una surface
	SDL_Surface* tmp = IMG_Load(path.c_str());
	if (!tmp)
		return { 0,0,1,1 };

	// Guardamos altura y anchura, y destruimos la surface
	int imgW = tmp->w;
	int imgH = tmp->h;
	SDL_DestroySurface(tmp);

	// Obtenemos la escala real del mapa
	float scaleX = renderedWidth / imgW;
	float scaleY = renderedHeight / imgH;

	// Devolvemos todos los valores
	return { imgW, imgH, scaleX, scaleY };
}