#include "SDLWindow.h"
GLRT_BEGIN
bool SDLWindow::setWindow(const char* title = "My SDLWindow", int x = SDL_WINDOWPOS_CENTERED, int y = SDL_WINDOWPOS_CENTERED, int w = 1280, int h = 720, SDL_WindowFlags flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI))
{
	if (!this->isValid) {
		LOG_ERROR( "SDLWindow::setWindow(): Window is not valid");
		return false;
	}
	this->m_width = w;
	this->m_height = h;
	this->m_x = x;
	this->m_y = y;
	this->m_title = title;
	this->m_Window = SDL_CreateWindow(title, x, y, w, h, flags);

	return true;
}

int SDLWindow::updateWindowSurface()
{
	if (m_Window)
		return SDL_UpdateWindowSurface(m_Window);
	return -1;
}

Surface::Ptr SDLWindow::getWindowSurface()
{
	if (!m_Window) {
		LOG_ERROR("SDLWindow::getWindowSurface(): Window is not valid");
		return nullptr;
	}
	auto tmp=Surface::acquire();
	tmp->setSurface(SDL_GetWindowSurface(m_Window));
	return tmp;
}

void SDLWindow::showWindow()
{
	SDL_ShowWindow(m_Window);
}

void SDLWindow::hideWindow()
{
	SDL_HideWindow(m_Window);
}

GLRT_END