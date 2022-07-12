#pragma once
#include "ERGBColor.h"
#include "EMath.h"

using namespace Elite;
struct HitInfo;

class Material
{
public:
	Material() = default;
	virtual ~Material() = default;

	virtual RGBColor Shade(const HitInfo& hitInfo, const FVector3& w0, const FVector3& w1)const = 0;
private:
};

class LambertMaterial :public Material
{
public:
	LambertMaterial(float diffuseReflectance, const RGBColor& diffuseColor);
	LambertMaterial();
	virtual ~LambertMaterial() = default;

	virtual RGBColor Shade(const HitInfo& hitInfo, const FVector3& w0, const FVector3& w1)const override;
	RGBColor GetDiffuseColor()const;
protected:
	float m_DiffuseReflectance;
	RGBColor m_DiffuseColor;
};

class Material_LambertPhong final : public LambertMaterial
{
public:
	Material_LambertPhong(float diffuseReflectance, const RGBColor& diffuseColor, float phongExponent);

	virtual ~Material_LambertPhong() = default;
	virtual RGBColor Shade(const HitInfo& hitInfo, const FVector3& w0, const FVector3& w1)const override;

private:
	float m_SpecularReflectance;
	float m_PhongExponent;
};

class Material_PBR final :public LambertMaterial
{
public:
	Material_PBR(const RGBColor& diffuseColor, bool isMetal, float roughness, const RGBColor& albedoRGB = { 0,0,0 });

	virtual ~Material_PBR() = default;
	virtual RGBColor Shade(const HitInfo& hitInfo, const FVector3& w0, const FVector3& w1)const override;
private:
	RGBColor m_AlbedoRGB;
	bool m_IsMetal;
	float m_Alpha;
};
