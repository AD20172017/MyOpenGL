//#include<iostream>

//#include "Def.h"
//#include "Object.h"
//SDL
//#include <SDL2/SDL.h>
//#include "SDLWindow.h"

//Core
//#include "Core/KeyEvent.h"
//#include "Core/MouseEvent.h"
//Test
//#include "SDLTest.h"
//#include "SDLWinTest.h"
//#include "SDLSufaceTest.h"
//#include "ECSTest.h"
//#include "GLSLTest.h"

#include <time.h>
#include <math.h>
#include <string>

#include "SDL2/SDL.h"
#include "GL/gl3w.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"
#include "tinydir/tinydir.h"

#include "GLSL/core/Scene.h"
#include "GLSL/loaders/Loader.h"
#include "GLSL/loaders/GLTFLoader.h"
#include "GLSL/core/Renderer.h"
#include "GLSL/tests/boyTestScene.h"
#include "GLSL/tests/ajaxTestScene.h"
#include "GLSL/tests/cornellTestScene.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include "tchar.h"

std::wstring string_to_wstring(const std::string& str) {
	return std::wstring(str.begin(), str.end());
}
std::string wstring_to_string(const std::wstring& wstr) {
	return std::string(wstr.begin(), wstr.end());
}
using namespace std;
namespace GLSLPT {

	GLSLPT::Scene* scene = nullptr;
	Renderer* renderer = nullptr;

	std::vector<string> sceneFiles;
	std::vector<string> envMaps;

	float mouseSensitivity = 0.01f;
	bool keyPressed = false;
	int sampleSceneIdx = 0;
	int selectedInstance = 0;
	double lastTime = SDL_GetTicks();
	int envMapIdx = 0;
	bool done = false;

	std::string shadersDir = "src/GLSL/shaders/";
	std::string assetsDir = "Resources/assets/";
	std::string envMapDir = "Resources/assets/HDR/";

	RenderOptions renderOptions;

	struct LoopData
	{
		SDL_Window* mWindow = nullptr;
		SDL_GLContext mGLContext = nullptr;
	};
	//文件加入sceneFile
	void GetSceneFiles()
	{
		tinydir_dir dir;
		int i;
		tinydir_open_sorted(&dir, wstring(assetsDir.begin(), assetsDir.end()).c_str());
		std::wcout <<  ( wstring(assetsDir.begin(), assetsDir.end()).c_str() ) << std::endl;

		for ( i = 0; i < dir.n_files; i++ )
		{
			tinydir_file file;
			tinydir_readfile_n(&dir, &file, i);

			std::wstring ext = std::wstring( file.extension);
			if ( ext == L"scene" || ext == L"gltf" || ext == L"glb" )
			{
				sceneFiles.push_back(assetsDir + wstring_to_string(std::wstring(file.name)));
			}
		}

		tinydir_close(&dir);
	}
	//添加环境贴图到envMaps
	void GetEnvMaps()
	{
		tinydir_dir dir;
		int i;
		tinydir_open_sorted(&dir,  wstring(envMapDir.begin(), envMapDir.end()).c_str());

		for ( i = 0; i < dir.n_files; i++ )
		{
			tinydir_file file;
			tinydir_readfile_n(&dir, &file, i);

			std::wstring ext = std::wstring( file.extension);
			if ( ext == L"hdr" )
			{
				envMaps.push_back(envMapDir + wstring_to_string(std::wstring(file.name)));
			}
		}

		tinydir_close(&dir);
	}

	void LoadScene(std::string sceneName)
	{
		delete scene;
		scene = new GLSLPT::Scene();
		std::wstring ext = string_to_wstring(sceneName.substr(sceneName.find_last_of(".") + 1));

		bool success = false;
		Mat4 xform;

		if ( ext == L"scene" )
			success = LoadSceneFromFile(sceneName, scene, renderOptions);
		else if ( ext == L"gltf" )
			success = LoadGLTF(sceneName, scene, renderOptions, xform, false);
		else if ( ext == L"glb" )
			success = LoadGLTF(sceneName, scene, renderOptions, xform, true);

		if ( !success )
		{
			printf("Unable to load scene\n");
			exit(0);
		}

		//loadCornellTestScene(scene, renderOptions);
		selectedInstance = 0;

		// Add a default HDR if there are no lights in the scene
		if ( !scene->envMap && !envMaps.empty() )
		{
			scene->AddEnvMap(envMaps[ envMapIdx ]);
			renderOptions.enableEnvMap = scene->lights.empty() ? true : false;
			renderOptions.envMapIntensity = 1.5f;
		}

		scene->renderOptions = renderOptions;
	}

	bool InitRenderer()
	{
		delete renderer;
		renderer = new Renderer(scene, shadersDir);
		return true;
	}

	void SaveFrame(const std::string filename)
	{
		unsigned char* data = nullptr;
		int w, h;
		renderer->GetOutputBuffer(&data, w, h);
		stbi_flip_vertically_on_write(true);
		stbi_write_png(filename.c_str(), w, h, 4, data, w * 4);
		printf("Frame saved: %s\n", filename.c_str());
		delete[ ] data;
	}

	void Render()
	{
		renderer->Render();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, renderOptions.windowResolution.x, renderOptions.windowResolution.y);
		renderer->Present();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Update(float secondsElapsed)
	{
		keyPressed = false;

		if ( !ImGui::GetIO().WantCaptureMouse && ImGui::IsAnyMouseDown() && !ImGuizmo::IsOver() )
		{
			if ( ImGui::IsMouseDown(0) )
			{
				ImVec2 mouseDelta = ImGui::GetMouseDragDelta(0, 0);
				scene->camera->OffsetOrientation(mouseDelta.x, mouseDelta.y);
				ImGui::ResetMouseDragDelta(0);
			}
			else if ( ImGui::IsMouseDown(1) )
			{
				ImVec2 mouseDelta = ImGui::GetMouseDragDelta(1, 0);
				scene->camera->SetRadius(mouseSensitivity * mouseDelta.y);
				ImGui::ResetMouseDragDelta(1);
			}
			else if ( ImGui::IsMouseDown(2) )
			{
				ImVec2 mouseDelta = ImGui::GetMouseDragDelta(2, 0);
				scene->camera->Strafe(mouseSensitivity * mouseDelta.x, mouseSensitivity * mouseDelta.y);
				ImGui::ResetMouseDragDelta(2);
			}
			scene->dirty = true;
		}

		renderer->Update(secondsElapsed);
	}

	void EditTransform(const float* view, const float* projection, float* matrix)
	{
		static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

		if ( ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE) )
		{
			mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		}

		ImGui::SameLine();
		if ( ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE) )
		{
			mCurrentGizmoOperation = ImGuizmo::ROTATE;
		}

		ImGui::SameLine();
		if ( ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE) )
		{
			mCurrentGizmoOperation = ImGuizmo::SCALE;
		}

		float matrixTranslation[ 3 ], matrixRotation[ 3 ], matrixScale[ 3 ];
		ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

		if ( mCurrentGizmoOperation != ImGuizmo::SCALE )
		{
			if ( ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL) )
			{
				mCurrentGizmoMode = ImGuizmo::LOCAL;
			}

			ImGui::SameLine();
			if ( ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD) )
			{
				mCurrentGizmoMode = ImGuizmo::WORLD;
			}
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(view, projection, mCurrentGizmoOperation, mCurrentGizmoMode, matrix, NULL, NULL);
	}

	void MainLoop(void* arg)
	{
		LoopData& loopdata = *( LoopData* ) arg;

		SDL_Event event;
		while ( SDL_PollEvent(&event) )
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if ( event.type == SDL_QUIT )
			{
				done = true;
			}
			if ( event.type == SDL_WINDOWEVENT )
			{
				if ( event.window.event == SDL_WINDOWEVENT_RESIZED )
				{
					renderOptions.windowResolution = iVec2(event.window.data1, event.window.data2);
					int w, h;
					SDL_GL_GetDrawableSize(loopdata.mWindow, &w, &h);
					renderOptions.windowResolution.x = w;
					renderOptions.windowResolution.y = h;

					if ( !renderOptions.independentRenderSize )
						renderOptions.renderResolution = renderOptions.windowResolution;

					scene->renderOptions = renderOptions;
					renderer->ResizeRenderer();
				}

				if ( event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(loopdata.mWindow) )
				{
					done = true;
				}
			}
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(loopdata.mWindow);
		ImGui::NewFrame();
		ImGuizmo::SetOrthographic(false);

		ImGuizmo::BeginFrame();
		{
			ImGui::Begin("Settings");

			ImGui::Text("Samples: %d ", renderer->GetSampleCount());

			ImGui::BulletText("LMB + drag to rotate");
			ImGui::BulletText("MMB + drag to pan");
			ImGui::BulletText("RMB + drag to zoom in/out");
			ImGui::BulletText("CTRL + click on a slider to edit its value");

			if ( ImGui::Button("Save Screenshot") )
			{
				SaveFrame("./img_" + to_string(renderer->GetSampleCount()) + ".png");
			}

			// Scenes
			std::vector<const char*> scenes;
			for ( int i = 0; i < sceneFiles.size(); ++i )
				scenes.push_back(sceneFiles[ i ].c_str());

			if ( ImGui::Combo("Scene", &sampleSceneIdx, scenes.data(), scenes.size()) )
			{
				LoadScene(sceneFiles[ sampleSceneIdx ]);
				SDL_RestoreWindow(loopdata.mWindow);
				SDL_SetWindowSize(loopdata.mWindow, renderOptions.windowResolution.x, renderOptions.windowResolution.y);
				int w, h;
				SDL_GL_GetDrawableSize(loopdata.mWindow, &w, &h);
				renderOptions.windowResolution.x = w;
				renderOptions.windowResolution.y = h;
				InitRenderer();
			}

			// Environment maps
			std::vector<const char*> envMapsList;
			for ( int i = 0; i < envMaps.size(); ++i )
				envMapsList.push_back(envMaps[ i ].c_str());

			if ( ImGui::Combo("EnvMaps", &envMapIdx, envMapsList.data(), envMapsList.size()) )
			{
				scene->AddEnvMap(envMaps[ envMapIdx ]);
			}

			bool optionsChanged = false;
			bool reloadShaders = false;

			optionsChanged |= ImGui::SliderFloat("Mouse Sensitivity", &mouseSensitivity, 0.001f, 1.0f);

			if ( ImGui::CollapsingHeader("Render Settings") )
			{
				optionsChanged |= ImGui::SliderInt("Max Spp", &renderOptions.maxSpp, -1, 256);
				optionsChanged |= ImGui::SliderInt("Max Depth", &renderOptions.maxDepth, 1, 10);

				reloadShaders |= ImGui::Checkbox("Enable Russian Roulette", &renderOptions.enableRR);
				reloadShaders |= ImGui::SliderInt("Russian Roulette Depth", &renderOptions.RRDepth, 1, 10);
				reloadShaders |= ImGui::Checkbox("Enable Roughness Mollification", &renderOptions.enableRoughnessMollification);
				optionsChanged |= ImGui::SliderFloat("Roughness Mollification Amount", &renderOptions.roughnessMollificationAmt, 0, 1);
				reloadShaders |= ImGui::Checkbox("Enable Volume MIS", &renderOptions.enableVolumeMIS);
			}

			if ( ImGui::CollapsingHeader("Environment") )
			{
				reloadShaders |= ImGui::Checkbox("Enable Uniform Light", &renderOptions.enableUniformLight);

				Vec3 uniformLightCol = Vec3::Pow(renderOptions.uniformLightCol, 1.0 / 2.2);
				optionsChanged |= ImGui::ColorEdit3("Uniform Light Color (Gamma Corrected)", ( float* ) ( &uniformLightCol ), 0);
				renderOptions.uniformLightCol = Vec3::Pow(uniformLightCol, 2.2);

				reloadShaders |= ImGui::Checkbox("Enable Environment Map", &renderOptions.enableEnvMap);
				optionsChanged |= ImGui::SliderFloat("Enviornment Map Intensity", &renderOptions.envMapIntensity, 0.1f, 10.0f);
				optionsChanged |= ImGui::SliderFloat("Enviornment Map Rotation", &renderOptions.envMapRot, 0.0f, 360.0f);
				reloadShaders |= ImGui::Checkbox("Hide Emitters", &renderOptions.hideEmitters);
				reloadShaders |= ImGui::Checkbox("Enable Background", &renderOptions.enableBackground);
				optionsChanged |= ImGui::ColorEdit3("Background Color", ( float* ) &renderOptions.backgroundCol, 0);
				reloadShaders |= ImGui::Checkbox("Transparent Background", &renderOptions.transparentBackground);
			}

			if ( ImGui::CollapsingHeader("Tonemapping") )
			{
				ImGui::Checkbox("Enable Tonemap", &renderOptions.enableTonemap);

				if ( renderOptions.enableTonemap )
				{
					ImGui::Checkbox("Enable ACES", &renderOptions.enableAces);
					if ( renderOptions.enableAces )
						ImGui::Checkbox("Simple ACES Fit", &renderOptions.simpleAcesFit);
				}
			}

			if ( ImGui::CollapsingHeader("Denoiser") )
			{

				ImGui::Checkbox("Enable Denoiser", &renderOptions.enableDenoiser);
				ImGui::SliderInt("Number of Frames to skip", &renderOptions.denoiserFrameCnt, 5, 50);
			}

			if ( ImGui::CollapsingHeader("Camera") )
			{
				float fov = Math::Degrees(scene->camera->fov);
				float aperture = scene->camera->aperture * 1000.0f;
				optionsChanged |= ImGui::SliderFloat("Fov", &fov, 10, 90);
				scene->camera->SetFov(fov);
				optionsChanged |= ImGui::SliderFloat("Aperture", &aperture, 0.0f, 10.8f);
				scene->camera->aperture = aperture / 1000.0f;
				optionsChanged |= ImGui::SliderFloat("Focal Distance", &scene->camera->focalDist, 0.01f, 50.0f);
				ImGui::Text("Pos: %.2f, %.2f, %.2f", scene->camera->position.x, scene->camera->position.y, scene->camera->position.z);
			}

			if ( ImGui::CollapsingHeader("Objects") )
			{
				bool objectPropChanged = false;

				std::vector<std::string> listboxItems;
				for ( int i = 0; i < scene->meshInstances.size(); i++ )
				{
					listboxItems.push_back(scene->meshInstances[ i ].name);
				}

				// Object Selection
				ImGui::ListBoxHeader("Instances");
				for ( int i = 0; i < scene->meshInstances.size(); i++ )
				{
					bool is_selected = selectedInstance == i;
					if ( ImGui::Selectable(listboxItems[ i ].c_str(), is_selected) )
					{
						selectedInstance = i;
					}
				}
				ImGui::ListBoxFooter();

				ImGui::Separator();
				ImGui::Text("Materials");

				// Material Properties
				Material* mat = &scene->materials[ scene->meshInstances[ selectedInstance ].materialID ];

				// Gamma correction for color picker. Internally, the renderer uses linear RGB values for colors
				Vec3 albedo = Vec3::Pow(mat->baseColor, 1.0 / 2.2);
				objectPropChanged |= ImGui::ColorEdit3("Albedo (Gamma Corrected)", ( float* ) ( &albedo ), 0);
				mat->baseColor = Vec3::Pow(albedo, 2.2);

				objectPropChanged |= ImGui::SliderFloat("Metallic", &mat->metallic, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("Roughness", &mat->roughness, 0.001f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("SpecularTint", &mat->specularTint, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("Subsurface", &mat->subsurface, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("Anisotropic", &mat->anisotropic, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("Sheen", &mat->sheen, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("SheenTint", &mat->sheenTint, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("Clearcoat", &mat->clearcoat, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("ClearcoatGloss", &mat->clearcoatGloss, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("SpecTrans", &mat->specTrans, 0.0f, 1.0f);
				objectPropChanged |= ImGui::SliderFloat("Ior", &mat->ior, 1.001f, 2.0f);

				int mediumType = ( int ) mat->mediumType;
				if ( ImGui::Combo("Medium Type", &mediumType, "None\0Absorb\0Scatter\0Emissive\0") )
				{
					reloadShaders = true;
					objectPropChanged = true;
					mat->mediumType = mediumType;
				}

				if ( mediumType != MediumType::None )
				{
					Vec3 mediumColor = Vec3::Pow(mat->mediumColor, 1.0 / 2.2);
					objectPropChanged |= ImGui::ColorEdit3("Medium Color (Gamma Corrected)", ( float* ) ( &mediumColor ), 0);
					mat->mediumColor = Vec3::Pow(mediumColor, 2.2);

					objectPropChanged |= ImGui::SliderFloat("Medium Density", &mat->mediumDensity, 0.0f, 5.0f);

					if ( mediumType == MediumType::Scatter )
						objectPropChanged |= ImGui::SliderFloat("Medium Anisotropy", &mat->mediumAnisotropy, -0.9f, 0.9f);
				}

				int alphaMode = ( int ) mat->alphaMode;
				if ( ImGui::Combo("Alpha Mode", &alphaMode, "Opaque\0Blend") )
				{
					reloadShaders = true;
					objectPropChanged = true;
					mat->alphaMode = alphaMode;
				}

				if ( alphaMode != AlphaMode::Opaque )
					objectPropChanged |= ImGui::SliderFloat("Opacity", &mat->opacity, 0.0f, 1.0f);

				// Transforms
				ImGui::Separator();
				ImGui::Text("Transforms");
				{
					float viewMatrix[ 16 ];
					float projMatrix[ 16 ];

					auto io = ImGui::GetIO();
					scene->camera->ComputeViewProjectionMatrix(viewMatrix, projMatrix, io.DisplaySize.x / io.DisplaySize.y);
					Mat4 xform = scene->meshInstances[ selectedInstance ].transform;

					EditTransform(viewMatrix, projMatrix, ( float* ) &xform);

					if ( memcmp(&xform, &scene->meshInstances[ selectedInstance ].transform, sizeof(float) * 16) )
					{
						scene->meshInstances[ selectedInstance ].transform = xform;
						objectPropChanged = true;
					}
				}

				if ( objectPropChanged )
					scene->RebuildInstances();
			}

			scene->renderOptions = renderOptions;

			if ( optionsChanged )
				scene->dirty = true;

			if ( reloadShaders )
			{
				scene->dirty = true;
				renderer->ReloadShaders();
			}

			ImGui::End();

			//printf("MaxSpp: %d Current Spp: %d Progress: %.1f%%   \r", scene->renderOptions.maxSpp, renderer->GetSampleCount(), renderer->GetProgress());
		}

		double presentTime = SDL_GetTicks();
		Update(( float ) ( presentTime - lastTime ));
		lastTime = presentTime;
		glClearColor(0., 0., 0., 0.);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		Render();
		SDL_GL_SwapWindow(loopdata.mWindow);
	}

	void GLSL(int argc, char** argv)
	{
		//使用时间生成随机数种子
		srand(( unsigned int ) time(0));

		std::string sceneFile;

		//解析输入参数
		for ( int i = 1; i < argc; ++i )
		{
			const std::string arg(argv[ i ]);
			if ( arg == "-s" || arg == "--scene" )
			{
				sceneFile = argv[ ++i ];
			}
			else if ( arg[ 0 ] == '-' )
			{
				printf("Unknown option %s \n'", arg.c_str());
				exit(0);
			}
		}

		if ( !sceneFile.empty() )
		{
			scene = new Scene();
			GetEnvMaps();
			LoadScene(sceneFile);
		}
		else
		{
			GetSceneFiles();
			GetEnvMaps();
			LoadScene(sceneFiles[ sampleSceneIdx ]);
		}

		// Setup SDL
		if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0 )
		{
			printf("Error: %s\n", SDL_GetError());
			return;
		}

		LoopData loopdata;

#ifdef __APPLE__
		// GL 3.2 Core + GLSL 150
		const char* glsl_version = "#version 150";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
		// GL 3.0 + GLSL 130
		const char* glsl_version = "#version 130";
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

		// Create window with graphics context
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);
		SDL_WindowFlags window_flags = ( SDL_WindowFlags ) ( SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI );
		loopdata.mWindow = SDL_CreateWindow("GLSL PathTracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, renderOptions.windowResolution.x, renderOptions.windowResolution.y, window_flags);

		// Query actual drawable window size
		int w, h;
		SDL_GL_GetDrawableSize(loopdata.mWindow, &w, &h);
		renderOptions.windowResolution.x = w;
		renderOptions.windowResolution.y = h;

		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

		loopdata.mGLContext = SDL_GL_CreateContext(loopdata.mWindow);
		if ( !loopdata.mGLContext )
		{
			fprintf(stderr, "Failed to initialize GL context!\n");
			return;
		}
		SDL_GL_SetSwapInterval(0); // Disable vsync

		// Initialize OpenGL loader
#if GL_VERSION_3_2
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
		bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
		bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
		bool err = gladLoadGL() == 0;
#endif
		if ( err )
		{
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			return;
		}
#endif

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); ( void ) io;

		ImGui_ImplSDL2_InitForOpenGL(loopdata.mWindow, loopdata.mGLContext);

		ImGui_ImplOpenGL3_Init(glsl_version);

		ImGui::StyleColorsDark();
		if ( !InitRenderer() )
			return;

		while ( !done )
		{
			MainLoop(&loopdata);
		}

		delete renderer;
		delete scene;

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();

		SDL_GL_DeleteContext(loopdata.mGLContext);
		SDL_DestroyWindow(loopdata.mWindow);
		SDL_Quit();
	}

};
int main(int argc, char** argv)

{
#ifdef _UNICODE
	std::wcout << L"_UNICODE is defined." << std::endl;
#else
	std::cout << "_UNICODE is not defined." << std::endl;
#endif
	//Core::KeyPressedEvent::Ptr ke= Core::KeyPressedEvent::acquire(Core::Key::A);
	//LOG_TRACE(ke->ToString());
	//{

	//Core::MouseButtonPressedEvent be(Core::Mouse::Button0);
	//LOG_TRACE(be.ToString());

	//}
	////TEST::SDLWin();
	////TEST::SurfaceTest();
	////TEST::ECSTest();
	//{
	//	auto obj = GLRT::Object::acquire();
	//}
	//GLRT::ObjectPool::GetInstance()->clearPool();
	GLSLPT::GLSL(argc, argv);

	return 0;
}
