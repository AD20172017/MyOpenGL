#include<iostream>
#include<GL/glew.h>
#include <SDL2/SDL.h>

#include "def.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
struct LoopData
{
	SDL_Window* mWindow = nullptr;
	SDL_GLContext mGLContext = nullptr;
};


GLuint g_uiVAO;
GLuint g_uiVBO;
GLuint g_uiMainProgram;


static const char* gp_cSeverity[] = { "High", "Medium", "Low", "Notification" };
static const char* gp_cType[] = { "Error", "Deprecated", "Undefined", "Portability", "Performance", "Other" };
static const char* gp_cSource[] = { "OpenGL", "OS", "GLSL Compiler", "3rd Party", "Application", "Other" };

void APIENTRY DebugCallback(uint32_t uiSource, uint32_t uiType, uint32_t uiID, uint32_t uiSeverity, int32_t iLength, const char* p_cMessage, void* p_UserParam)
{
	// Get the severity
	uint32_t uiSevID = 3;
	switch (uiSeverity) {
	case GL_DEBUG_SEVERITY_HIGH:
		uiSevID = 0; break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		uiSevID = 1; break;
	case GL_DEBUG_SEVERITY_LOW:
		uiSevID = 2; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
	default:
		uiSevID = 3; break;
	}

	// Get the type
	uint32_t uiTypeID = 5;
	switch (uiType) {
	case GL_DEBUG_TYPE_ERROR:
		uiTypeID = 0; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		uiTypeID = 1; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		uiTypeID = 2; break;
	case GL_DEBUG_TYPE_PORTABILITY:
		uiTypeID = 3; break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		uiTypeID = 4; break;
	case GL_DEBUG_TYPE_OTHER:
	default:
		uiTypeID = 5; break;
	}

	// Get the source
	uint32_t uiSourceID = 5;
	switch (uiSource) {
	case GL_DEBUG_SOURCE_API:
		uiSourceID = 0; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
		uiSourceID = 1; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER:
		uiSourceID = 2; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:
		uiSourceID = 3; break;
	case GL_DEBUG_SOURCE_APPLICATION:
		uiSourceID = 4; break;
	case GL_DEBUG_SOURCE_OTHER:
	default:
		uiSourceID = 5; break;
	}

	// Output to the Log
	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Debug: Severity=%s, Type=%s, Source=%s - %s",
		gp_cSeverity[uiSevID], gp_cType[uiTypeID], gp_cSource[uiSourceID], p_cMessage);
	if (uiSeverity == GL_DEBUG_SEVERITY_HIGH) {
		//This a serious error so we need to shutdown the program
		SDL_Event event;
		event.type = SDL_QUIT;
		SDL_PushEvent(&event);
	}
}

void GLDebug_Init()
{
	//Allow for synchronous callbacks.
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	//Set up the debug info callback
	glDebugMessageCallback((GLDEBUGPROC)&DebugCallback, NULL);

	//Set up the type of debug information we want to receive
	uint32_t uiUnusedIDs = 0;
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &uiUnusedIDs, GL_TRUE); //Enable all
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); //Disable notifications
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, NULL, GL_FALSE); //Disable low severity
	//glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, NULL, GL_FALSE); //Disable medium severity warnings
}
bool GL_LoadShader(GLuint& uiShader, GLenum ShaderType, const GLchar* p_cShader)
{
	// Build and link the shader program
	uiShader = glCreateShader(ShaderType);
	glShaderSource(uiShader, 1, &p_cShader, NULL);
	glCompileShader(uiShader);

	// Check for errors
	GLint iTestReturn;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iTestReturn);
	if (iTestReturn == GL_FALSE) {
		GLchar p_cInfoLog[1024];
		int32_t iErrorLength;
		glGetShaderInfoLog(uiShader, 1024, &iErrorLength, p_cInfoLog);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to compile shader: %s\n", p_cInfoLog);
		glDeleteShader(uiShader);
		return false;
	}
	return true;
}

bool GL_LoadShaders(GLuint& uiShader, GLuint uiVertexShader, GLuint uiFragmentShader)
{
	// Link the shaders
	uiShader = glCreateProgram();
	glAttachShader(uiShader, uiVertexShader);
	glAttachShader(uiShader, uiFragmentShader);
	glLinkProgram(uiShader);

	//Check for error in link
	GLint iTestReturn;
	glGetProgramiv(uiShader, GL_LINK_STATUS, &iTestReturn);
	if (iTestReturn == GL_FALSE) {
		GLchar p_cInfoLog[1024];
		int32_t iErrorLength;
		glGetShaderInfoLog(uiShader, 1024, &iErrorLength, p_cInfoLog);
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to link shaders: %s\n", p_cInfoLog);
		glDeleteProgram(uiShader);
		return false;
	}
	return true;
}

bool GL_LoadShaderFile(GLuint& uiShader, GLenum ShaderType, const char* p_cFileName, int iFileID)
{
#ifdef _WIN32
	// Can load directly from windows resource file
	HINSTANCE hInst = GetModuleHandle(NULL);
	HRSRC hRes = FindResource(hInst, MAKEINTRESOURCE(iFileID), RT_RCDATA);
	HGLOBAL hMem = LoadResource(hInst, hRes);
	DWORD size = SizeofResource(hInst, hRes);
	char* resText = (char*)LockResource(hMem);

	// Load in the shader
	bool bReturn = GL_LoadShader(uiShader, ShaderType, resText);

	// Print the shader name to assist debugging
	if (!bReturn)
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, " Failed shader: %s\n", p_cFileName);

	// Close Resource
	FreeResource(hMem);

	return bReturn;
#else
	// Load directly from the specified file
	SDL_RWops* p_File = SDL_RWFromFile(p_cFileName, "r");
	if (p_File == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to open shader file: %s\n", p_cFileName);
		return false;
	}

	// Allocate a string to store contents
	size_t lFileSize = (size_t)SDL_RWsize(p_File);
	GLchar* p_cFileContents = (GLchar*)malloc(lFileSize + 1);
	SDL_RWread(p_File, p_cFileContents, lFileSize, 1);
	p_cFileContents[lFileSize] = '\0'; // Add terminating character

	// Load in the shader
	bool bReturn = GL_LoadShader(uiShader, ShaderType, p_cFileContents);

	// Print the shader name to assist debugging
	if (!bReturn)
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, " Failed shader: %s\n", p_cFileName);

	// Close file
	SDL_RWclose(p_File);

	return bReturn;
#endif
}
bool GL_Init()
{
	// Initialize GLEW
	glewExperimental = GL_TRUE; // Allow experimental or pre-release drivers to return all supported extensions
	GLenum GlewError = glewInit();
	if (GlewError != GLEW_OK) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize GLEW: %s\n", glewGetErrorString(GlewError));
		return false;
	}

#ifdef _DEBUG
	// Initialise debug callback
	GLDebug_Init();
#endif

	// Set up initial GL attributes
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set the cleared back buffer to black
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Set the cleared back buffer to blue
	glCullFace(GL_BACK);                  // Set back-face culling
	glEnable(GL_CULL_FACE);               // Enable use of back/front face culling
	glEnable(GL_DEPTH_TEST);              // Enable use of depth testing
	glDisable(GL_STENCIL_TEST);           // Disable stencil test for speed

	//Create vertex shader source
	const GLchar p_cVertexShaderSource[] = {
		"#version 430 core\n \
	        layout(location = 0) in vec2 v2VertexPos2D;\n \
	        void main() \n \
	        {\n \
	            gl_Position = vec4(v2VertexPos2D, 0.0f, 1.0f);\n \
	        }"
	};

	// Create vertex shader
	GLuint uiVertexShader;
	if (!GL_LoadShader(uiVertexShader, GL_VERTEX_SHADER, p_cVertexShaderSource))
		return false;

	//Create fragment shader source
	const GLchar p_cFragmentShaderSource[] = {
		"#version 430 core\n \
	        out vec3 v3FragOutput;\n \
	        void main() \n \
	        {\n \
	            //v3FragOutput = vec3(1.0f, 1.0f, 1.0f);\n \
	            v3FragOutput = vec3(1.0f, 1.0f, 0.0f);\n \
	        }"
	};

	// Create fragment shader
	GLuint uiFragmentShader;
	if (!GL_LoadShader(uiFragmentShader, GL_FRAGMENT_SHADER, p_cFragmentShaderSource))
		return false;


	// Create program
	if (!GL_LoadShaders(g_uiMainProgram, uiVertexShader, uiFragmentShader))
		return false;

	// Clean up unneeded shaders
	glDeleteShader(uiVertexShader);
	glDeleteShader(uiFragmentShader);

	// Create a Vertex Array Object
	glGenVertexArrays(1, &g_uiVAO);
	if (g_uiVAO == 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to generate VAO.");
		return false;
	}
	glBindVertexArray(g_uiVAO);

	// Create VBO data
	GLfloat fVertexData[] =
	{
		-0.5f, 0.5f,
		 0.0f, -0.5f,
		 0.5f, 0.5f
	};

	// Create Vertex Buffer Object
	glGenBuffers(1, &g_uiVBO);
	if (g_uiVBO == 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to generate VBO.");
		return false;
	}
	glBindBuffer(GL_ARRAY_BUFFER, g_uiVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fVertexData), fVertexData, GL_STATIC_DRAW);

	// Specify location of data within buffer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (const GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Specify program to use
	glUseProgram(g_uiMainProgram);

	return true;
}

void GL_Quit()
{
	// Release the shader program
	glDeleteProgram(g_uiMainProgram);

	// Delete VBO and VAO
	glDeleteBuffers(1, &g_uiVBO);
	glDeleteVertexArrays(1, &g_uiVAO);
}

void GL_Render()
{
	// Clear the render output and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Specify VAO to use
	glBindVertexArray(g_uiVAO);

	// Draw the triangle
	glDrawArrays(GL_TRIANGLES, 0, 3);
}


int main(int argc, char** argv)

{

	std::cout << "My OpenGL raytracing!" << std::endl;

	//Init SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize SDL: %s\n", SDL_GetError());
		return 1;
	}

	const char* glsl_version = "#version 330";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	// Create window with graphics context
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	//自动设置窗口分辨率
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

	LoopData loopdata;


	loopdata.mWindow = SDL_CreateWindow("MyOpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
	if (loopdata.mWindow == NULL) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}


	

	loopdata.mGLContext = SDL_GL_CreateContext(loopdata.mWindow);
	if (!loopdata.mGLContext)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Failed to create OpenGL context: %s\n", SDL_GetError());
		SDL_DestroyWindow(loopdata.mWindow);
		SDL_Quit();
		return 1;
	}
	//V-Sync 0关闭 1打卡 -1立即交换
	SDL_GL_SetSwapInterval(-1);

	//Initialize OpenGL
	if (GL_Init()) {
		// *** Add message pump code here ***
		// Start the program message pump
		SDL_Event Event;
		bool bQuit = false;
		while (!bQuit) {
			// Poll SDL for buffered events
			while (SDL_PollEvent(&Event)) {
				if (Event.type == SDL_QUIT)
					bQuit = true;
				else if (Event.type == SDL_KEYDOWN) {
					if (Event.key.keysym.sym == SDLK_ESCAPE)
						bQuit = true;
				}
			}

			// Render the scene
			GL_Render();

			// Swap the back-buffer and present it
			SDL_GL_SwapWindow(loopdata.mWindow);
		}
		// Delete any created GL resources
		GL_Quit();
	}

	

	SDL_GL_DeleteContext(loopdata.mGLContext);
	SDL_DestroyWindow(loopdata.mWindow);
	SDL_Quit();

	return 0;
}
