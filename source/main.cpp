//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "ETimer.h"
#include "ERenderer.h"
#include "PerspectiveCamera.h"
#include "LightManager.h"
#include "Scenegraph.h"
#include "OBJ_Parser.h"

//Forward Declarations
void InitLights(LightManager* pLightManager);
void InitGeneralScene(int idx);
void InitBunnyScene(int idx);
void ToggleScene(int& sceneIdx);

void ShutDown(SDL_Window* pWindow)
{
	SDL_DestroyWindow(pWindow);
	SDL_Quit();
}

int main(int argc, char* args[])
{
	//Unreferenced parameters
	(void)argc;
	(void)args;

	//Create window + surfaces
	SDL_Init(SDL_INIT_VIDEO);

	const uint32_t width = 640;
	const uint32_t height = 480;
	SDL_Window* pWindow = SDL_CreateWindow(
		"RayTracer - **Tristan Wauthier**",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width, height, 0);

	if (!pWindow)
		return 1;

	//Initialize "framework"
	Elite::Timer* pTimer = new Elite::Timer();
	LightManager lightManager{};
	Elite::Renderer* pRenderer = new Elite::Renderer(pWindow, &lightManager);
	PerspectiveCamera camera{ FPoint3{0.f,0.f,7.5f},45.f };

	//Initialize lights
	InitLights(&lightManager);

	//Initialize scenes
	int generalScene = Scenegraph::getInstance().AddScene();
	int bunnySceneIDX = Scenegraph::getInstance().AddScene();
	InitGeneralScene(generalScene);
	InitBunnyScene(bunnySceneIDX);

	int currentScene{ 0 };

	//Start loop
	pTimer->Start();
	float printTimer = 0.f;
	bool isLooping = true;
	bool takeScreenshot = false;

	//ProcessMouseMovement
	int x{}, y{};

	float angle{ 0 };
	const float rotSpeed{ 60.f };

	std::cout << "To toggle between scenes, press TAB\n";

	while (isLooping)
	{
		//--------- Get input events --.-------
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				if (e.key.keysym.scancode == SDL_SCANCODE_X)
					takeScreenshot = true;
				if (e.key.keysym.scancode == SDL_SCANCODE_T)
					pRenderer->ToggleModes();
				if (e.key.keysym.scancode == SDL_SCANCODE_Z)
					pRenderer->ToggleHardShadows();
				if (e.key.keysym.scancode == SDL_SCANCODE_J)
					lightManager.ToggleDirectionalLights();
				if (e.key.keysym.scancode == SDL_SCANCODE_K)
					lightManager.TogglePointLight(0);
				if (e.key.keysym.scancode == SDL_SCANCODE_L)
					lightManager.TogglePointLight(1);
				if (e.key.keysym.scancode == SDL_SCANCODE_TAB)
					ToggleScene(currentScene);
				break;
			}
		}

		//MouseButtons
		if (SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT) && SDL_GetRelativeMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			camera.TranslateCamera(0.f, float(-y / 10.f), 0.f);
		}
		else if (SDL_GetMouseState(NULL, NULL) == SDL_BUTTON_RMASK)
		{
			camera.RotateXCamera(y / 2.f);
			camera.RotateYCamera(x / 2.f);
		}
		else if (SDL_GetRelativeMouseState(NULL, NULL) == (SDL_BUTTON_LMASK))
		{
			if (x > 0.5 || x < -0.5)
			{
				camera.RotateYCamera(x / 2.f);
			}
			if (y > 0.5 || y < -0.5)
			{
				FVector3 translation{ camera.GetForwardVector() * float(y) };
				camera.TranslateCamera(translation.x / 2.f, translation.y / 2.f, translation.z / 2.f);
			}
		}

		//--------- Update --------
		angle += rotSpeed * pTimer->GetElapsed();

		FMatrix4 rotationMatrix = MakeRotationY(ToRadians(angle));

		Scenegraph::getInstance().RotateTriangles(rotationMatrix, generalScene);
		Scenegraph::getInstance().UpdateTriangles(generalScene);

		//--------- Render ---------
		//render box
		pRenderer->Render(camera, currentScene);

		//--------- Timer ---------
		pTimer->Update();
		printTimer += pTimer->GetElapsed();
		if (printTimer >= 1.f)
		{
			printTimer = 0.f;
			std::cout << "FPS: " << pTimer->GetFPS() << std::endl;
		}

		//Save screenshot after full render
		if (takeScreenshot)
		{
			if (!pRenderer->SaveBackbufferToImage())
				std::cout << "Screenshot saved!" << std::endl;
			else
				std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
			takeScreenshot = false;
		}
	}
	pTimer->Stop();

	//Shutdown "framework"
	delete pRenderer;
	delete pTimer;

	ShutDown(pWindow);
	return 0;
}

void InitLights(LightManager* pLightManager)
{
	//Initialize lights
	pLightManager->AddItem(FPoint3(-5.f, 5.f, 5.f), RGBColor(0.788f, 0.886f, 1.f), 100.f, true);
	pLightManager->AddItem(FPoint3(5.f, 5.f, 5.f), RGBColor(1.f, 0.576f, 0.161f), 100.f, true);
	pLightManager->AddItem(FPoint3(0.f, 9.f, 0.f), RGBColor(1.f, 0.839f, 0.667f), 100.f, true);

	pLightManager->AddItem(FVector3(0.f, 0.f, -1.f), RGBColor(1, 1, 1), 0.4f, true);
}

void InitGeneralScene(int idx)
{
	//Walls
	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 0.f,-2.f,0.f }, FVector3{ 0.f,1.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 0.f,10.f,0.f }, FVector3{ 0.f,-1.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);

	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 0.f,0.f,-15.f }, FVector3{ 0.f,0.f, 1.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);

	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 7.5f,0.f,0.f }, FVector3{ -1.f,0.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ -7.5f,0.f,0.f }, FVector3{ 1.f,0.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);

	//bottom row
	Scenegraph::getInstance().AddObjectToGraph(new Sphere(FPoint3{ -1.25f,-0.25f,0.f }, new Material_PBR(RGBColor(1.f, 1.f, 1.f), true, 1.f, RGBColor{ 0.972f,0.960f,0.915f }), 0.5f), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Sphere(FPoint3{ -0.f,-0.25f,0.f }, new Material_PBR(RGBColor(1.f, 1.f, 1.f), true, 0.6f, RGBColor{ 0.972f,0.960f,0.915f }), 0.5f), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Sphere(FPoint3{ 1.25f,-0.25f,0.f }, new Material_PBR(RGBColor(1.f, 1.f, 1.f), true, 0.1f, RGBColor{ 0.972f,0.960f,0.915f }), 0.5f), idx);

	//top row
	Scenegraph::getInstance().AddObjectToGraph(new Sphere(FPoint3{ -1.25f,1.f,0.f }, new Material_PBR(RGBColor(0.138f, 0.277f, 0), false, 1.f), 0.5f), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Sphere(FPoint3{ -0.f,1.f,0.f }, new Material_PBR(RGBColor(0.f, 0.309f, 0.104f), false, 0.6f), 0.5f), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Sphere(FPoint3{ 1.25f,1.f,0.f }, new Material_PBR(RGBColor(0.f, 0.125f, 0.172f), false, 0.1f), 0.5f), idx);

	//Triangles
	Scenegraph::getInstance().AddObjectToGraph(new Triangle({ {-1.75f,2.75f,0.f},{-1.75f,1.75f,0},{-0.75f,1.75f,0.f} }, new Material_PBR(RGBColor(0.f, 0.125f, 0.172f), false, 0.4f), Triangle::Culling::FrontFace), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Triangle({ {-0.5f,2.75f,0.f},{-0.5f,1.75f,0},{0.5f,1.75f,0.f} }, new Material_PBR(RGBColor(0.f, 0.125f, 0.172f), false, 0.4f), Triangle::Culling::BackFace), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Triangle({ {0.75f,2.75f,0.f},{0.75f,1.75f,0},{1.75f,1.75f,0.f} }, new Material_PBR(RGBColor(0.f, 0.125f, 0.172f), false, 0.4f), Triangle::Culling::NoCulling), idx);
}

void InitBunnyScene(int idx)
{
	//Walls
	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 0.f,-2.f,0.f }, FVector3{ 0.f,1.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 0.f,10.f,0.f }, FVector3{ 0.f,-1.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);

	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 0.f,0.f,-15.f }, FVector3{ 0.f,0.f, 1.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);

	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ 7.5f,0.f,0.f }, FVector3{ -1.f,0.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);
	Scenegraph::getInstance().AddObjectToGraph(new Plane(FPoint3{ -7.5f,0.f,0.f }, FVector3{ 1.f,0.f, 0.f }, new Material_LambertPhong(0.6f, RGBColor(1.f, 1.f, 1.f), 60.f)), idx);

	std::vector<FPoint3> vertices{  };
	std::vector<int> indices{  };
	std::vector<FVector3> normals{  };

	OBJ_Parser::ParseOBJ("lowpoly_bunny.obj", vertices, indices);
	FMatrix4 transform = FMatrix4::Identity();
	transform[3] = FVector4(0, -2, 0, 1);
	Scenegraph::getInstance().AddObjectToGraph(new TriangleMesh(vertices, indices, normals, transform, Triangle::Culling::BackFace, new LambertMaterial(1.f, { 1,0,0 })), idx);
	Scenegraph::getInstance().UpdateTriangleMeshes(idx);
}

void ToggleScene(int& sceneIdx)
{
	int amountOfScenes = Scenegraph::getInstance().GetAmountOfScenes();
	++sceneIdx;
	if (sceneIdx >= amountOfScenes)
	{
		sceneIdx = 0;
	}
}