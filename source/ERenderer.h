/*=============================================================================*/
// Copyright 2017-2019 Elite Engine
// Authors: Matthieu Delaere
/*=============================================================================*/
// ERenderer.h: class that holds the surface to render to, does traverse the pixels
// and traces the rays using a tracer
/*=============================================================================*/
#ifndef ELITE_RAYTRACING_RENDERER
#define	ELITE_RAYTRACING_RENDERER

#include <cstdint>
#include "EMath.h"
#include "Ray.h"
#include "HitInfo.h"
#include <vector>
#include "Scenegraph.h"
#include "PerspectiveCamera.h"

struct SDL_Window;
struct SDL_Surface;
class PointLight;
class LightManager;

namespace Elite
{
	class Renderer final
	{
	public:
		Renderer(SDL_Window* pWindow, LightManager* pLightManager);
		~Renderer();

		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&) noexcept = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer& operator=(Renderer&&) noexcept = delete;

		void Render(const PerspectiveCamera& currentCamera, int sceneIdx);

		bool SaveBackbufferToImage() const;
		FPoint3 ConvertRasterToScreen(uint32_t col, uint32_t row, float fov)const;
		void ToggleModes();
		void ToggleHardShadows();
		enum class Mode
		{
			IrradianceOnly,
			BRDFOnly,
			All
		};
	private:
		SDL_Window* m_pWindow = nullptr;
		SDL_Surface* m_pFrontBuffer = nullptr;
		SDL_Surface* m_pBackBuffer = nullptr;
		uint32_t* m_pBackBufferPixels = nullptr;
		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		float m_AspectRatio;
		LightManager* m_pLightManager;

		Mode m_Mode = Mode::All;
		bool m_UseHardShadows = true;
		bool CheckHits(const Ray& ray, HitInfo& hitInfo, const std::vector<BaseShape*>& shapeVector, bool isShadow)const;

		void RenderScreen(const std::vector<BaseShape*>& shapeVector, float fov, const FPoint3& cameraPos, const FMatrix4& ONB, int threadNumber, int maxAmountOfThreads);
	};
}

#endif