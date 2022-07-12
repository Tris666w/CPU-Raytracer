#include "Sphere.h"
#include <iostream>
#include "Material.h"

Sphere::~Sphere()
{
	BaseShape::~BaseShape();
	delete m_pMaterial;
}
Sphere::Sphere(FPoint3 center, Material* pMaterial, float radius)
	:m_Radius{ radius }
	, BaseShape(center)
	, m_pMaterial{ pMaterial }
{
	//m_pMaterial = new Material_LambertPhong(0.85f, color, 60);
}

bool Sphere::Hit(const Ray& ray, HitInfo& hitRecord, bool isShadowRay)const
{
	FVector3 vectorRaySphere = { ray.origin - m_Origin };

	float a, b, c{};
	a = Dot(ray.direction, ray.direction);
	b = Dot(2 * ray.direction, vectorRaySphere);
	c = Dot(vectorRaySphere, vectorRaySphere) - Square(m_Radius);

	float discriminant = b * b - 4 * a * c;
	if (discriminant <= 0)
	{
		return false;
	}

	float tempT = (-b - sqrt(discriminant)) / (2 * a);
	if (tempT >= ray.tMax)
	{
		return false;
	}
	if (tempT > ray.tMin && tempT < ray.tMax)
	{
		hitRecord.hitTValue = tempT;
		hitRecord.hitPos = ray.origin + tempT * ray.direction;
		hitRecord.hitpMaterial = m_pMaterial;
		hitRecord.hitNormal = GetNormalized(hitRecord.hitPos - m_Origin);
		return true;
	}
	else
	{
		tempT = (-b + sqrt(discriminant)) / (2 * a);
		if (tempT > ray.tMin && tempT < ray.tMax)
		{
			hitRecord.hitTValue = tempT;
			hitRecord.hitPos = ray.origin + tempT * ray.direction;
			hitRecord.hitpMaterial = m_pMaterial;
			hitRecord.hitNormal = GetNormalized(hitRecord.hitPos - m_Origin);

			return true;
		}
		else
		{
			return false;
		}
	}
}