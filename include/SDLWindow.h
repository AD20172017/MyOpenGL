#pragma once
#include "Object.h"
GLRT_BEGIN

class SDLWindow :
	public Object
{
	OBJECT_SETUP(SDLWindow)
public:
	ACQUIRE_FUNC(SDLWindow)

	bool init()override {
		return true;
	}
};

GLRT_END

