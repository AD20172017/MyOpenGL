#include "Surface.h"
GLRT_BEGIN

int Surface::blitSurface(const SDL_Rect* srcRect, Ptr dest, SDL_Rect* destRect)
{
	return SDL_BlitSurface(m_Surface, srcRect, dest->m_Surface, destRect);
}

Surface::Ptr Surface::convertSurface(const SDL_PixelFormat* fmt, Uint32 flags)
{
	auto tmp=Surface::acquire();
	tmp->setSurface(SDL_ConvertSurface(m_Surface, fmt, flags));
	return tmp;
}

int Surface::fillRect(const SDL_Rect* rect, Uint32 color)
{
	return SDL_FillRect(m_Surface, rect, color);
}

Uint32 Surface::mapRGB(Uint8 r, Uint8 g, Uint8 b)const
{
	return SDL_MapRGB(m_Surface->format, r, g, b);
}

Uint32 Surface::mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)const
{
	return SDL_MapRGBA(m_Surface->format, r, g, b, a);
}

int Surface::setColorKey(Uint32 colorkey)const
{
	return SDL_SetColorKey(m_Surface, SDL_TRUE, colorkey);
}

Uint32 Surface::getARGB(int x, int y)const
{
	int index = y * m_Surface->w + x;
	Uint32* pixels = (Uint32*)m_Surface->pixels;

	return pixels[index];
}

int Surface::setSurfaceBlendMode(SDL_BlendMode mode)
{
	return SDL_SetSurfaceBlendMode(m_Surface, mode);
}

int Surface::getSurfaceBlendMode(SDL_BlendMode* mode)
{
	return SDL_GetSurfaceBlendMode(m_Surface, mode);
}

SDL_Surface* Surface::getSDL_Surface()const
{
	return m_Surface;
}




GLRT_END


