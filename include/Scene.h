#pragma once
#include "Marcos.h"
#include "Camera.h"
#include <iostream>
GLRT_BEGIN

	class Scene
	{
	public:
		Scene():m_camera(nullptr) {
			std::cout << "Created Scene" << std::endl;
		}

		~Scene() {
		};
	
		Camera* m_camera;



	};

GLRT_END


