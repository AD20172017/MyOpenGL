#include "WindowManager.h"
#include "ECS/Coordinator.h"
#include <iostream>

using namespace ECS;
extern Coordinator gCoordinator;
GLRT_BEGIN


void WindowManager::Init(std::string const& windowTitle, unsigned int windowWidth, unsigned int windowHeight, unsigned int windowPositionX, unsigned int windowPositionY)
{
	glfwInit();

	m_Window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	glfwWindowHint(GLFW_FOCUSED, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	glfwMakeContextCurrent(m_Window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(2);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

}

void WindowManager::Update()
{
	glfwSwapBuffers(m_Window);
}

void WindowManager::ProcessEvents()
{
	glfwPollEvents();

	bool buttonStateChanged = true;

	if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE))
	{
		gCoordinator.SendEvent(Events::Window::QUIT);
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_W))
	{
		m_buttons.set(static_cast<std::size_t>(InputButtons::W));
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_A))
	{
		m_buttons.set(static_cast<std::size_t>(InputButtons::A));
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_S))
	{
		m_buttons.set(static_cast<std::size_t>(InputButtons::S));
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_D))
	{
		m_buttons.set(static_cast<std::size_t>(InputButtons::D));
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_Q))
	{
		m_buttons.set(static_cast<std::size_t>(InputButtons::Q));
	}
	else if (glfwGetKey(m_Window, GLFW_KEY_E))
	{
		m_buttons.set(static_cast<std::size_t>(InputButtons::E));
	}
	else
	{
		buttonStateChanged = false;
	}

	if (buttonStateChanged)
	{
		Event event(Events::Window::INPUT);
		event.SetParam(Events::Window::Input::INPUT, m_buttons);
		gCoordinator.SendEvent(event);
	}

}

void WindowManager::Shutdown()
{
	glfwDestroyWindow(m_Window);
	glfwTerminate();
}


GLRT_END
