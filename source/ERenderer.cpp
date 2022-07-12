//External includes
#include "SDL.h"
#include "SDL_surface.h"
#include <iostream>
#include <thread>

//Project includes
#include "ERenderer.h"
#include "ERGBColor.h"
#include "Sphere.h"
#include "Plane.h"
#include "Lights.h"
#include "EMathUtilities.h"
#include "LightManager.h"

Elite::Renderer::Renderer(SDL_Window* pWindow, LightManager* pLightManager)
{
	//Initialize
	m_pWindow = pWindow;
	m_pFrontBuffer = SDL_GetWindowSurface(pWindow);
	int width, height = 0;
	SDL_GetWindowSize(pWindow, &width, &height);
	m_Width = static_cast<uint32_t>(width);
	m_Height = static_cast<uint32_t>(height);
	m_pBackBuffer = SDL_CreateRGBSurface(0, m_Width, m_Height, 32, 0, 0, 0, 0);
	m_pBackBufferPixels = (uint32_t*)m_pBackBuffer->pixels;
	m_AspectRatio = float(m_Width) / float(m_Height);
	m_pLightManager = pLightManager;
}

Elite::Renderer::~Renderer()
{
}

void Elite::Renderer::Render(const PerspectiveCamera& currentCamera, int sceneIdx)
{
	SDL_LockSurface(m_pBackBuffer);

	std::vector<BaseShape*> shapeVector{ Scenegraph::getInstance().GetObjectVector(sceneIdx) };

	float fov{ currentCamera.GetFOV() };
	FPoint3 cameraPos{ currentCamera.GetPosition() };
	FMatrix4 ONB = currentCamera.GetONBMatrix();

	RenderScreen(shapeVector, fov, cameraPos, ONB, 0, 1);

	SDL_UnlockSurface(m_pBackBuffer);
	SDL_BlitSurface(m_pBackBuffer, 0, m_pFrontBuffer, 0);
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Elite::Renderer::SaveBackbufferToImage() const
{
	return SDL_SaveBMP(m_pBackBuffer, "BackbufferRender.bmp");
}

FPoint3 Elite::Renderer::ConvertRasterToScreen(uint32_t col, uint32_t row, float fov) const
{
	FPoint3 convertedPoint{};
	convertedPoint.x = ((2 * (col + 0.5f) / m_Width) - 1) * m_AspectRatio * fov;
	convertedPoint.y = (1 - 2 * (row + 0.5f) / m_Height) * fov;
	convertedPoint.z = -1;
	return convertedPoint;
}

void Elite::Renderer::ToggleModes()
{
	switch (m_Mode)
	{
	case Elite::Renderer::Mode::IrradianceOnly:
		m_Mode = Mode::BRDFOnly;
		break;
	case Elite::Renderer::Mode::BRDFOnly:
		m_Mode = Mode::All;
		break;
	case Elite::Renderer::Mode::All:
		m_Mode = Mode::IrradianceOnly;
		break;
	}
}

void Elite::Renderer::ToggleHardShadows()
{
	m_UseHardShadows = !m_UseHardShadows;
}

bool Elite::Renderer::CheckHits(const Ray& ray, HitInfo& hitInfo, const std::vector<BaseShape*>& shapeVector, bool isShadowRay) const
{
	HitInfo currentHR{};
	hitInfo.hitTValue = FLT_MAX;
	bool hasHit{ false };

	for (const BaseShape* pCurrentShape : shapeVector)
	{
		if (pCurrentShape->Hit(ray, currentHR, isShadowRay) && currentHR.hitTValue < hitInfo.hitTValue)
		{
			hasHit = true;
			hitInfo = currentHR;
		}
	}
	return hasHit;
}

void Elite::Renderer::RenderScreen(const std::vector<BaseShape*>& shapeVector, float fov, const FPoint3& cameraPos, const FMatrix4& ONB, int threadNumber, int maxAmountOfThreads)
{
	for (int r = (threadNumber / maxAmountOfThreads) * m_Height; r < int(((threadNumber + 1) / maxAmountOfThreads) * m_Height); ++r)
	{
		for (int c = 0; c < int(m_Width); ++c)
		{
			//Initialize the ray
			FPoint4 cssPos = ConvertRasterToScreen(c, r, fov);
			cssPos = ONB * cssPos;
			Ray ray{ cameraPos, GetNormalized(FVector3(cssPos - FPoint4(cameraPos))) };
			Normalize(ray.direction);
			HitInfo hitInfo{};
			RGBColor pixelColor{};
			float lambertAdjust{};

			if (CheckHits(ray, hitInfo, shapeVector, false))
			{
				for (LightBase* currentLight : m_pLightManager->GetLightsVector())
				{
					if (currentLight->GetIsEnabled())
					{
						Ray shadowRay{ hitInfo.hitPos, FVector3{-currentLight->GetLightDirection(hitInfo)} };
						HitInfo shadowRayHit{};
						currentLight->SetRayTMax(hitInfo, shadowRay);
						if (m_UseHardShadows)
						{
							if (!CheckHits(shadowRay, shadowRayHit, shapeVector, true))
							{
								float lambertAdjust = Dot(hitInfo.hitNormal, -currentLight->GetLightDirection(hitInfo));
								if (lambertAdjust > 0)
								{
									FVector3 w0{ -currentLight->GetLightDirection(hitInfo) };
									FVector3 w1{ GetNormalized(FVector3(cameraPos - hitInfo.hitPos)) };
									switch (m_Mode)
									{
									case Elite::Renderer::Mode::IrradianceOnly:
										pixelColor += (currentLight->CalculateRadiance(hitInfo.hitPos) * lambertAdjust);
										break;
									case Elite::Renderer::Mode::BRDFOnly:
										pixelColor += (hitInfo.hitpMaterial->Shade(hitInfo, w0, w1) * lambertAdjust);
										break;
									case Elite::Renderer::Mode::All:
										pixelColor += (currentLight->CalculateRadiance(hitInfo.hitPos) * hitInfo.hitpMaterial->Shade(hitInfo, w0, w1) * lambertAdjust);
										break;
									}
								}
							}
						}
						else
						{
							lambertAdjust = Dot(hitInfo.hitNormal, -currentLight->GetLightDirection(hitInfo));
							if (lambertAdjust > 0)
							{
								FVector3 w0{ -currentLight->GetLightDirection(hitInfo) };
								FVector3 w1{ GetNormalized(FVector3(cameraPos - hitInfo.hitPos)) };
								switch (m_Mode)
								{
								case Elite::Renderer::Mode::IrradianceOnly:
									pixelColor += (currentLight->CalculateRadiance(hitInfo.hitPos) * lambertAdjust);
									break;
								case Elite::Renderer::Mode::BRDFOnly:
									pixelColor += (hitInfo.hitpMaterial->Shade(hitInfo, w0, w1) * lambertAdjust);
									break;
								case Elite::Renderer::Mode::All:
									pixelColor += (currentLight->CalculateRadiance(hitInfo.hitPos) * hitInfo.hitpMaterial->Shade(hitInfo, w0, w1) * lambertAdjust);
									break;
								}
							}
						}
					}
				}

				pixelColor.MaxToOne();
				m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format,
					static_cast<uint8_t>(pixelColor.r * 255.f),
					static_cast<uint8_t>(pixelColor.g * 255.f),
					static_cast<uint8_t>(pixelColor.b * 255.f));
			}
			else
			{
				m_pBackBufferPixels[c + (r * m_Width)] = SDL_MapRGB(m_pBackBuffer->format, uint32_t(0.f), uint32_t(0.f), uint32_t(0.f));
			}
		}
	}
}