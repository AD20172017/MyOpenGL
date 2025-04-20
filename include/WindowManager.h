#pragma once
#include "Marcos.h"
#include <string>
//Ææ¹ÖµÄË³Ðò
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <bitset>

GLRT_BEGIN
class WindowManager
{
public:
	void Init(std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight,
		unsigned int windowPositionX, unsigned int windowPositionY);
	void Update();
	void ProcessEvents();

	void Shutdown();

private:
	GLFWwindow* m_Window;
	std::bitset<8>m_buttons;
};

GLRT_END

