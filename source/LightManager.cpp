#include "LightManager.h"
#include <iostream>
LightManager::~LightManager()
{
	for (LightBase* currentLight : m_pLightVector)
	{
		delete currentLight;
	}
}

void LightManager::AddItem(const FPoint3& pos, const RGBColor& color, float lightIntensity, bool isEnabled)
{
	m_pLightVector.push_back(new PointLight(pos, color, lightIntensity, isEnabled));
}

void LightManager::AddItem(const FVector3& direction, const RGBColor& color, float lightIntensity, bool isEnabled)
{
	m_pLightVector.push_back(new DirectionalLight(direction, color, lightIntensity, isEnabled));
}

std::vector<LightBase*> LightManager::GetLightsVector() const
{
	return m_pLightVector;
}

size_t LightManager::Size() const
{
	return m_pLightVector.size();
}

void LightManager::ToggleDirectionalLights()
{
	DirectionalLight* pDirectionalLight{};
	for (LightBase* pLight : m_pLightVector)
	{
		pDirectionalLight = dynamic_cast<DirectionalLight*>(pLight);
		if (pDirectionalLight != nullptr)
		{
			bool state = (pLight->GetIsEnabled());
			if (state)
			{
				pLight->SetIsEnabled(false);
			}
			else
			{
				pLight->SetIsEnabled(true);
			}
		}
	}
}

void LightManager::TogglePointLight(int pointLightID)
{
	int counter{ 0 };
	PointLight* pPointLight{};
	for (LightBase* pLight : m_pLightVector)
	{
		if (counter == pointLightID)
		{
			pPointLight = dynamic_cast<PointLight*>(pLight);
			if (pPointLight != nullptr)
			{
				bool state = (pLight->GetIsEnabled());
				if (state)
				{
					pLight->SetIsEnabled(false);
				}
				else
				{
					pLight->SetIsEnabled(true);
				}
				return;
			}
		}
		else
		{
			++counter;
		}
	}
}