#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

static int height;
static int width;
int main(int argc, char** argv) {

	std::cout << "My OpenGL raytracing!" << std::endl;

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window;
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}




	return 0;
}
