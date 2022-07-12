#include "BRDF.h"

Elite::RGBColor BRDF::Lambert(float diffuseReflectance, const Elite::RGBColor& m_DiffuseColor)
{
	return m_DiffuseColor * (diffuseReflectance / float(E_PI));
}

Elite::RGBColor BRDF::Lambert(const RGBColor& diffuseReflectance, const Elite::RGBColor& m_DiffuseColor)
{
	return m_DiffuseColor * (diffuseReflectance / float(E_PI));
}

Elite::RGBColor BRDF::Phong(float specularReflectance, float phongExponent, const FVector3& l, const FVector3& v, const FVector3& normal)
{
	FVector3 r = Elite::Reflect(l, normal);
	float cosAngle = Dot(r, v);
	float phongSpecular = specularReflectance * pow(cosAngle, phongExponent);
	return Elite::RGBColor(phongSpecular, phongSpecular, phongSpecular);
	return RGBColor();
}

Elite::RGBColor BRDF::CookTorrance(const FVector3& n, const FVector3& v, const FVector3& l, float D, const RGBColor& F, float G)
{
	RGBColor color{};
	color = (RGBColor(D, D, D) * F * G) / (4 * Dot(v, n) * Dot(l, n));
	return color;
}

float BRDF::TrowbridgeReitzGGX(const FVector3& normal, const FVector3& halfVector, float alpha)
{
	//float num = Square(roughnessSquared);
	//float dotSq = Square(Dot(normal, halfVector));
	//float denom = float((E_PI)*Square((dotSq) * (num - 1) + 1));
	//return num / denom;
	return Square(alpha) / (float(E_PI) * Square(Square(Dot(normal, halfVector)) * (Square(alpha) - 1) + 1));
}

Elite::RGBColor BRDF::Schlick(const FVector3& h, const FVector3& v, const Elite::RGBColor& f0)
{
	return f0 + (RGBColor(1, 1, 1) - f0) * pow(1 - Dot(h, v), 5);
}

float BRDF::SchlickGGX(const FVector3& n, const FVector3& dir, float k)
{
	float dot = Dot(n, dir);

	return dot / (dot * (1.f - k) + k);
}

float BRDF::SmithsMethod(const FVector3& n, const FVector3& v, const FVector3& l, float k)
{
	return SchlickGGX(n, v, k) * SchlickGGX(n, l, k);
}