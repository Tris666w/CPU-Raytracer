#pragma once
#include "ERGBColor.h"
#include "EMath.h"
using namespace Elite;
class BRDF
{
public:
	static Elite::RGBColor Lambert(float diffuseReflectance, const Elite::RGBColor& m_DiffuseColor);
	static Elite::RGBColor Lambert(const RGBColor& diffuseReflectance, const Elite::RGBColor& m_DiffuseColor);
	static Elite::RGBColor Phong(float specularReflectance, float phongExponent, const FVector3& l, const FVector3& v, const FVector3& normal);
	static Elite::RGBColor CookTorrance(const FVector3& n, const FVector3& v, const FVector3& l, float D, const RGBColor& F, float G);

	static float TrowbridgeReitzGGX(const FVector3& n, const FVector3& h, float roughnessSquared);
	static Elite::RGBColor Schlick(const FVector3& h, const FVector3& v, const Elite::RGBColor& f0);
	static float SchlickGGX(const FVector3& n, const FVector3& dir, float k);
	static float SmithsMethod(const FVector3& n, const FVector3& v, const FVector3& l, float k);
private:
};
