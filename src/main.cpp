#include<iostream>

//#include <SDL2/SDL.h>
#include "Def.h"
#include "Object.h"
//#include "SDLWindow.h"
#include "Core/KeyEvent.h"

//Test
//#include "SDLTest.h"
//#include "SDLWinTest.h"
#include "SDLSufaceTest.h"
//#include "ECSTest.h"



int main(int argc, char** argv)

{
	Core::KeyPressedEvent ke(Core::Key::A);

	//TEST::SDLWin();
	TEST::SurfaceTest();
	//TEST::ECSTest();
	{
	auto obj=GLRT::Object::acquire();
	}
	GLRT::ObjectPool::GetInstance()->clearPool();
	
	return 0;
}
