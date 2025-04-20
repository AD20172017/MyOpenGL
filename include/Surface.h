#pragma once
#include "Object.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
GLRT_BEGIN

class Renderer;
class Surface :
	public Object
{
	OBJECT_SETUP(Surface)
public:
	ACQUIRE_FUNC(Surface)


	bool init()override {

		return true;
	}

	bool clear()override {
		if (m_Surface == nullptr) {
			LOG_ERROR(std::cerr, "Surface::clear(): Surface is not valid");
			return false;
		}
		SDL_FreeSurface(m_Surface);
		m_Surface = nullptr;
		return true;
	}
	SDL_Surface* getSurface() { return m_Surface; }

	bool setSurface(SDL_Surface* surface) {
		if (surface == nullptr) {
			LOG_ERROR(std::cerr, "Surface::setSurface(): Surface is nullptr");
			return false;
		}
		m_Surface = surface;
		return true;
	}
	bool setSurface(const std::string& filepath) {

		m_Surface = IMG_Load(filepath.c_str());
		ASSERT(m_Surface, "Surface::setSurface(): Surface is nullptr\n"<<IMG_GetError());
		return true;
	}



	//���� ���и����������
	int blitSurface(const SDL_Rect* srcRect, Ptr dest, SDL_Rect* destRect);
	//SDL_ConvertSurface(SDL_Surface*src,const SDL_PixelFormat* fmt,Uint32flags)
	Ptr convertSurface(const SDL_PixelFormat* fmt, Uint32 flags);

	//������亯��
	int fillRect(const SDL_Rect* rect, Uint32 color);
	//���ùؼ�ɫ�����عؼ�ɫ
	Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b)const;
	Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)const;
	//���ùؼ�ɫ����pngͼƬ��ʽ������Ч
	int setColorKey(Uint32 colorkey)const;
	//��ö�Ӧλ�����ص�ARGBֵ
	Uint32 getARGB(int x, int y)const;
	//���ģʽ
	int setSurfaceBlendMode(SDL_BlendMode blendMode);
	int getSurfaceBlendMode(SDL_BlendMode* blendMode);

	SDL_Surface* getSDL_Surface()const;
	friend class Renderer;


private:
	SDL_Surface* m_Surface = nullptr;
	//SDL_Surface* m_pSurface = nullptr;
	//SDL_Renderer* m_pRenderer = nullptr;
	//SDL_Texture* m_pTexture = nullptr;
	//SDL_Rect m_Rect;
	//int m_Width, m_Height;
};

GLRT_END

