#pragma once
#include "Camera.h"
#include <iostream>
namespace GLRT {

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

}


