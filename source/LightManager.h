#pragma once
#include <vector>
#include "Lights.h"

class LightManager
{
public:
	LightManager() = default;
	~LightManager();

	LightManager(const LightManager&) = delete;
	LightManager& operator=(const LightManager&) = delete;
	LightManager(const LightManager&&) = delete;
	LightManager& operator=(const LightManager&&) = delete;

	//Add pointLight
	void AddItem(const FPoint3& pos, const RGBColor& color, float lightIntensity, bool isEnabled);
	//Add directional light
	void AddItem(const FVector3& direction, const RGBColor& color, float lightIntensity, bool isEnabled);

	std::vector<LightBase*> GetLightsVector()const;

	size_t Size()const;

	void ToggleDirectionalLights();
	void TogglePointLight(int pointLightID);

private:
	std::vector<LightBase*> m_pLightVector;
};
