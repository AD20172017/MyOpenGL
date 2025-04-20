#pragma once
#include <iostream>
#include "SDLWindow.h"
#include "Surface.h"
#include "SDL2/SDL.h"

using namespace GLRT;
using namespace std;
namespace TEST
{

	//��ʼ��
	void initlize();
	void clear();
	//����
	void draw(Surface::Ptr screen);
	//����
	void update(float dt);
	//�¼�����
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
			//��ʼ��ȫ����ϵͳ
			SDL_Init(SDL_INIT_EVERYTHING);
			IMG_Init(IMG_INIT_PNG);
			//��������
			SDLWindow::Ptr window = SDLWindow::acquire();
			window->setWindow("SDL_EngineTest", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, SDL_WINDOW_SHOWN);
			//��ȡ���ڶ�Ӧ��surface
			Surface::Ptr screen = window->getWindowSurface();

			initlize();
			//��Ϸѭ��
			while (g_bRunning)
			{
				Uint32 currentTick = SDL_GetTicks();
				if (currentTick >= nextTick)
				{
					//֡�ʹ̶�Ϊ�������
					float dt = secondsPerFrame;
					time += (currentTick - lastTick) / 1000.f;

					lastTick = currentTick;
					nextTick = lastTick + (Uint32)(secondsPerFrame * 1000);
					//��ʾ��ǰ֡��
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

	void handleEvents()
	{
		SDL_Event event = {};
		//���¼�������ѯ
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: g_bRunning = false; break;
			}
		}
	}
}