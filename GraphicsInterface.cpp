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

	int realW, realH;
	SDL_GetWindowSize(window, &realW, &realH);

	Parameters::screenWidth = realW;
	Parameters::screenHeight = realH;

	InitFontAndGlyphs();
}


void GraphicsInterface::InitFontAndGlyphs()
{
	TTF_Init();

	font = TTF_OpenFont("Commodore-64-v6.3.ttf", Parameters::bigFontSize);

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

	grenadeFont = TTF_OpenFont("WolveSymbol.ttf", Parameters::bigFontSize);

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
	for (auto pair : glyphs)
	{
		SDL_DestroyTexture(pair.second->glyphTexture);
		delete pair.second;
	}

	for (auto pair : grenadeGlyphs)
	{
		SDL_DestroyTexture(pair.second->glyphTexture);
		delete pair.second;
	}

	TTF_CloseFont(font);
	TTF_CloseFont(grenadeFont);
	TTF_Quit();
}

int GraphicsInterface::PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize)
{
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

int GraphicsInterface::PrintText(std::string text, int x, int y, bColor color, EFONT_SIZE fontsize, bool grenade)
{
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

GraphicsInterface::~GraphicsInterface()
{
	for (auto pair : image_collection)
		SDL_DestroyTexture(pair.second);

	DestroyFontAndGlyphs();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

bool GraphicsInterface::LoadImage(std::string path)
{
	SDL_Texture *txt= IMG_LoadTexture(renderer, path.c_str());

	image_collection.insert(std::make_pair(path, txt));

	return false;
}

bool GraphicsInterface::MustWindowClose()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) return true;
	}

	return false;
}

void GraphicsInterface::DrawSprite(std::string img_name, Transform transform, Vector2 size)
{
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
void GraphicsInterface::ClearScreen(unsigned char r, unsigned char g, unsigned char b)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, 255); // Negro
	SDL_RenderClear(renderer);
}

void GraphicsInterface::DrawFrame()
{
	SDL_RenderPresent(renderer);
}

void GraphicsInterface::DrawRectScreen(float x, float y, float w, float h,
	unsigned char r, unsigned char g, unsigned char b)
{
	SDL_FRect rect = { x, y, w, h };
	SDL_SetRenderDrawColor(renderer, r, g, b, 255);
	SDL_RenderFillRect(renderer, &rect);
}

std::vector<uint8_t> GraphicsInterface::LoadCollisionMask(const std::string& path, int& width, int& height)
{
	SDL_Surface* surf = IMG_Load(path.c_str());
	if (!surf) return {};

	int w = surf->w;
	int h = surf->h;

	width = w;
	height = h;

	std::vector<uint8_t> mask(w * h);

	// Obtener formato correcto para SDL3
	const SDL_PixelFormatDetails* fmt = SDL_GetPixelFormatDetails(surf->format);

	const SDL_Palette* pal = nullptr;

	Uint32* pixels = (Uint32*)surf->pixels;

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

	SDL_DestroySurface(surf);
	return mask;
}

MapInfo GraphicsInterface::GetMapInfo(const std::string& path, float renderedWidth, float renderedHeight)
{
	SDL_Surface* tmp = IMG_Load(path.c_str());
	if (!tmp)
		return { 0,0,1,1 };

	int imgW = tmp->w;
	int imgH = tmp->h;
	SDL_DestroySurface(tmp);

	float scaleX = renderedWidth / imgW;
	float scaleY = renderedHeight / imgH;

	return { imgW, imgH, scaleX, scaleY };
}
