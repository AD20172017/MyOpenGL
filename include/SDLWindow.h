#pragma once
#include "Object.h"
#include "SDL2/SDL.h"
#include <string>
#include "Surface.h"
GLRT_BEGIN
class Surface;
class Renderer;
class SDLWindow :
	public Object
{
	OBJECT_SETUP(SDLWindow)
public:
	ACQUIRE_FUNC(SDLWindow)

	bool init()override {

		return true;
	}
	bool clear()override {
		if (m_Window == nullptr) {
			LOG_ERROR(std::cerr, "SDLWindow::clear(): Window is not valid");
			return false;
		}
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
		return true;
	}
	bool setWindow(const char* title,int x, int y, int w, int h, SDL_WindowFlags flags);

	int updateWindowSurface();
	//��ô��ڵ�surface
	//�� typedef ������һ�������ռ��У�Ȼ������Ҫ�ĵط�ʹ�ø������ռ䡣
	Surface::Ptr getWindowSurface();
	//��ʾ����
	void showWindow();
	//���ش���
	void hideWindow();


	//��ô��ڴ�С
	//Size getWindowSize()const;
	//��ô�����ʵ��С
	//Size getRealWindowSize()const;
	//���´���λ�ò�����
	//SDL_Point getWindowPosition();
	//Renderer��Ϊ��Ԫ

private:
	SDL_Window* m_Window = nullptr;
	const char* m_title;
	int m_width;
	int m_height;
	int m_x;
	int m_y;

};

GLRT_END

