#include "Material.h"
#include "BRDF.h"
#include "HitInfo.h"
#include <iostream>

LambertMaterial::LambertMaterial(float diffuseReflectance, const RGBColor& diffuseColor)
	:m_DiffuseReflectance{ diffuseReflectance }
	, m_DiffuseColor{ diffuseColor }
{
}

LambertMaterial::LambertMaterial()
	: m_DiffuseReflectance{ -100.f }
	, m_DiffuseColor{ RGBColor{0.f,0.f,0.f} }
{
}

RGBColor LambertMaterial::Shade(const HitInfo& hitInfo, const FVector3& w0, const FVector3& w1) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
}

RGBColor LambertMaterial::GetDiffuseColor() const
{
	return m_DiffuseColor;
}

Material_LambertPhong::Material_LambertPhong(float diffuseReflectance, const RGBColor& diffuseColor, float phongExponent)
	:LambertMaterial(diffuseReflectance, diffuseColor)
	, m_SpecularReflectance{ 1 - diffuseReflectance }
	, m_PhongExponent{ phongExponent }
{
}

RGBColor Material_LambertPhong::Shade(const HitInfo& hitInfo, const FVector3& w0, const FVector3& w1) const
{
	return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::Phong(m_SpecularReflectance, m_PhongExponent, w0, w1, hitInfo.hitNormal);
}

Material_PBR::Material_PBR(const RGBColor& diffuseColor, bool isMetal, float roughness, const RGBColor& albedoRGB)
	:LambertMaterial(0, diffuseColor)
	, m_AlbedoRGB{ albedoRGB }
	, m_IsMetal{ isMetal }
	, m_Alpha{ Square(roughness) }
{
}

RGBColor Material_PBR::Shade(const HitInfo& hitInfo, const FVector3& l, const FVector3& v) const
{
	RGBColor f0{};
	if (m_IsMetal)
	{
		f0 = m_AlbedoRGB;
	}
	else
	{
		f0 = RGBColor{ 0.4f,0.4f,0.4f };
	}

	FVector3 h = (l + v) / Magnitude(l + v);

	RGBColor F = BRDF::Schlick(h, v, f0);

	float D = BRDF::TrowbridgeReitzGGX(hitInfo.hitNormal, h, m_Alpha);

	float k = Square(m_Alpha + 1) / 8.f;

	float G = BRDF::SmithsMethod(hitInfo.hitNormal, v, l, k);

	RGBColor specular = BRDF::CookTorrance(hitInfo.hitNormal, v, l, D, F, G);

	RGBColor kd{};
	if (!m_IsMetal)
	{
		kd = (RGBColor(1, 1, 1) - F);
	}
	else
	{
		kd = RGBColor{ 0,0,0 };
	}

	RGBColor diffuse = BRDF::Lambert(kd, m_DiffuseColor);

	return diffuse + specular;
}