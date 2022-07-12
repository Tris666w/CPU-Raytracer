#pragma once
#include "ERGBColor.h"
#include "EMath.h"
using namespace Elite;
struct HitInfo;
struct Ray;
class LightBase
{
public:
	LightBase(const RGBColor& color, float lightIntensity, bool isEnabled);
	virtual RGBColor CalculateRadiance(const FPoint3& pointToShade) = 0;
	virtual FVector3 GetLightDirection(const HitInfo& hitInfo)const = 0;
	virtual void SetRayTMax(const HitInfo& hitInfo, Ray& ray)const = 0;

	bool GetIsEnabled()const { return m_IsEnabled; }
	void SetIsEnabled(bool state) { m_IsEnabled = state; };

protected:
	RGBColor m_LightColor;
	float m_LightIntensity;
	bool m_IsEnabled;
private:
};

class PointLight final :public LightBase
{
public:
	PointLight(const FPoint3& pos, const RGBColor& color, float lightIntensity, bool isEnabled);
	RGBColor CalculateRadiance(const FPoint3& pointToShade) override;
	FVector3 GetLightDirection(const HitInfo& hitInfo)const override;
	virtual void SetRayTMax(const HitInfo& hitInfo, Ray& ray)const;

	FPoint3 GetPos()const;
private:
	FPoint3 m_Position;
};

class DirectionalLight final : public LightBase
{
public:
	DirectionalLight(const FVector3& direction, const RGBColor& color, float lightIntensity, bool isEnabled);
	RGBColor CalculateRadiance(const FPoint3& pointToShade) override;
	FVector3 GetLightDirection(const HitInfo& hitInfo)const override;
	virtual void SetRayTMax(const HitInfo& hitInfo, Ray& ray)const;
private:
	FVector3 m_Direction;
};
