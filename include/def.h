#pragma once
#include <string>
//#include<GL/glew.h>
//#include<GLFW/glfw3.h>

#define CONFIG_DEF const

// �ƻ�����ֻ��һЩ��������

namespace Config {

	// ���ڲ���
	CONFIG_DEF int WindowWidth = 720;
	CONFIG_DEF int WindowHeight = 400;

	// ������
	CONFIG_DEF float MouseSensitivity = 0.01f;

	// Shader ·��
	CONFIG_DEF std::string ShaderRootDir = "../shaders/";
	// ����ӳ��
	//CONFIG_DEF size_t moveKey[] = { GLFW_KEY_S, GLFW_KEY_W, GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_E, GLFW_KEY_Q };
	//CONFIG_DEF size_t arrowKey[] = { GLFW_KEY_DOWN, GLFW_KEY_UP, GLFW_KEY_RIGHT, GLFW_KEY_LEFT, GLFW_KEY_PAGE_DOWN, GLFW_KEY_PAGE_UP };

	// �����������Լ������...
};





