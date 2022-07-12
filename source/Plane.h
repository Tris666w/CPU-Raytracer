#pragma once
#include "BaseShape.h"
#include "EMath.h"
#include "ERGBColor.h"
class Material_LambertPhong;
class Plane final :public BaseShape
{
public:
	~Plane() override;
	Plane(FPoint3 pos, FVector3 normal, Material* pMaterial);
	bool Hit(const Ray& ray, HitInfo& hitInfo, bool isShadowRay)const override;

private:
	FVector3 m_Normal;
	Material* m_pMaterial;
};
