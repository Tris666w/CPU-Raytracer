#include "Lights.h"
#include "HitInfo.h"
#include "Ray.h"

LightBase::LightBase(const RGBColor& color, float lightIntensity, bool isEnabled)
	:m_LightColor{ color }
	, m_LightIntensity{ lightIntensity }
	, m_IsEnabled{ isEnabled }
{
}

PointLight::PointLight(const FPoint3& pos, const RGBColor& color, float lightIntensity, bool isEnabled)
	: LightBase{ color, lightIntensity ,isEnabled }
	, m_Position{ pos }

{
}

RGBColor PointLight::CalculateRadiance(const FPoint3& pointToShade)
{
	float distanceSquared{ Elite::SqrDistance(m_Position, pointToShade) };
	return m_LightColor * (m_LightIntensity / distanceSquared);
}

//Wrong formula
FVector3 PointLight::GetLightDirection(const HitInfo& hitInfo) const
{
	return GetNormalized(hitInfo.hitPos - m_Position);
}

void PointLight::SetRayTMax(const HitInfo& hitInfo, Ray& ray) const
{
	ray.tMax = Magnitude(hitInfo.hitPos - m_Position);
}

FPoint3 PointLight::GetPos() const
{
	return m_Position;
}

DirectionalLight::DirectionalLight(const FVector3& direction, const RGBColor& color, float lightIntensity, bool isEnabled)
	:LightBase{ color,lightIntensity ,isEnabled }
	, m_Direction{ Elite::GetNormalized(direction) }
{
}

RGBColor DirectionalLight::CalculateRadiance(const FPoint3& pointToShade)
{
	return m_LightColor * m_LightIntensity;
}

FVector3 DirectionalLight::GetLightDirection(const HitInfo& hitInfo) const
{
	return m_Direction;
}

void DirectionalLight::SetRayTMax(const HitInfo& hitInfo, Ray& ray) const
{
	return;
}