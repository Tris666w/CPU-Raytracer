#pragma once
#include "BaseShape.h"
class Material_LambertPhong;
class Sphere final : public BaseShape
{
public:
	~Sphere() override;
	Sphere(FPoint3 center, Material* pMaterial, float radius = 1.f);
	bool Hit(const Ray& ray, HitInfo& hitRecord, bool isShadowRay) const override;

private:
	float m_Radius;
	Material* m_pMaterial;
};
