#pragma once

#include <iostream>
#include "SDLWindow.h"
#include "SDL2/SDL.h"

#include <crtdbg.h>

using namespace GLRT;
using namespace std;

namespace TEST {

	//����
	void update(float dt);
	//�¼�����
	void handleEvents();

	bool g_bRunning = true;

	int SDLWin()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
		float secondsPerFrame = 1.f / 60.f;
		float time = 0.f;
		Uint32 nextTick = 0u;
		Uint32 lastTick = 0u;
		int currentFrame = 0;
		//��ʼ��ȫ����ϵͳ
		SDL_Init(SDL_INIT_EVERYTHING);
		//��������
		auto window = SDLWindow::acquire();
		window->setWindow("SDL_Engine test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 480, SDL_WINDOW_SHOWN);
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
				//TODO
				update(dt);
			}
			handleEvents();

		}

		_CrtDumpMemoryLeaks();
		return 0;
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