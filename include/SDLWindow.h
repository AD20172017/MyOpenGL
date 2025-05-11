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
			LOG_ERROR("SDLWindow::clear(): Window is not valid");
			return false;
		}
		SDL_DestroyWindow(m_Window);
		m_Window = nullptr;
		return true;
	}
	bool setWindow(const char* title,int x, int y, int w, int h, SDL_WindowFlags flags);

	int updateWindowSurface();
	//获得窗口的surface
	//将 typedef 定义在一个命名空间中，然后在需要的地方使用该命名空间。
	Surface::Ptr getWindowSurface();
	//显示窗口
	void showWindow();
	//隐藏窗口
	void hideWindow();


	//获得窗口大小
	//Size getWindowSize()const;
	//获得窗口真实大小
	//Size getRealWindowSize()const;
	//更新窗口位置并返回
	//SDL_Point getWindowPosition();
	//Renderer作为友元

private:
	SDL_Window* m_Window = nullptr;
	const char* m_title;
	int m_width;
	int m_height;
	int m_x;
	int m_y;

};

GLRT_END

