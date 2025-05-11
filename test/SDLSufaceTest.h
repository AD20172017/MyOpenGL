#pragma once
#include <iostream>
#include "SDLWindow.h"
#include "Surface.h"
#include "SDL2/SDL.h"

using namespace GLRT;
using namespace std;
namespace TEST
{

	//初始化
	void initlize();
	void clear();
	//绘制
	void draw(Surface::Ptr screen);
	//更新
	void update(float dt);
	//事件处理
	void handleEvents();

	bool g_bRunning = true;
	Surface::Ptr g_pSurface;

	int SurfaceTest()
	{
		{
			float secondsPerFrame = 1.f / 60.f;
			float time = 0.f;
			Uint32 nextTick = 0u;
			Uint32 lastTick = 0u;
			int currentFrame = 0;
			//初始化全部子系统
			SDL_Init(SDL_INIT_EVERYTHING);
			IMG_Init(IMG_INIT_PNG);
			//创建窗口
			SDLWindow::Ptr window = SDLWindow::acquire();
			window->setWindow("SDL_EngineTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, SDL_WINDOW_SHOWN);
			//获取窗口对应的surface
			Surface::Ptr screen = window->getWindowSurface();

			initlize();
			//游戏循环
			while (g_bRunning)
			{
				Uint32 currentTick = SDL_GetTicks();
				if (currentTick >= nextTick)
				{
					//帧率固定为方便调试
					float dt = secondsPerFrame;
					time += (currentTick - lastTick) / 1000.f;

					lastTick = currentTick;
					nextTick = lastTick + (Uint32)(secondsPerFrame * 1000);
					//显示当前帧数
					if (time >= 1.f)
					{
						time -= 1.f;
						printf("%d\n", currentFrame);
						currentFrame = 0;
					}
					else
						currentFrame++;

					update(dt);

					draw(screen);
					window->updateWindowSurface();
				}
				handleEvents();

			}



			IMG_Quit();
			SDL_Quit();
		}
		_CrtDumpMemoryLeaks();
		return 0;
	}

	void initlize()
	{
		g_pSurface = Surface::acquire();
		//g_pSurface->setSurface("D:/17656/Documents/GameEngine/MyOpenGL/Debug/FGO.jpg");
		g_pSurface->setSurface("Resources/FGO.jpg");

	}


	void draw(Surface::Ptr screen)
	{
		SDL_Rect rect = {};
		screen->fillRect(nullptr, 0x00ffff);

		SDL_Rect srcRect = { 0,0,94,48 };
		SDL_Rect destRect = { 100,100,94,48 };
		g_pSurface->blitSurface(&srcRect, screen, &destRect);
	}

	void update(float dt)
	{
	}

	void clear() {

	}
	void handleEvents()
	{
		SDL_Event event = {};
		//对事件进行轮询
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: g_bRunning = false; break;
			}
		}
	}
}