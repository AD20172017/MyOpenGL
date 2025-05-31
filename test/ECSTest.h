#pragma once
#include "Marcos.h"
#include "Coordinator.h"
#include "WindowManager.h"
#include <chrono>
using namespace GLRT;
using namespace std;
using namespace ECS;

namespace TEST {


	static bool quit = false;


	void QuitHandler(Event& event)
	{
		quit = true;
	}

	int ECSTest()
	{
		Coordinator::GetInstance().Init();


		WindowManager windowManager;
		windowManager.Init("Nexus", 1920, 1080, 0, 0);


		Coordinator::GetInstance().AddEventListener(FUNCTION_LISTENER(Events::Window::QUIT, QuitHandler));
		float dt = 0.0f;

		while (!quit)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			windowManager.ProcessEvents();

			

			windowManager.Update();

			auto stopTime = std::chrono::high_resolution_clock::now();

			dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
		}


		windowManager.Shutdown();

		return 0;
	}


};