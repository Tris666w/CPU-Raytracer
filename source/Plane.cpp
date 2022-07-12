#include "Plane.h"

Plane::~Plane()
{
	BaseShape::~BaseShape();
	delete m_pMaterial;
}

Plane::Plane(FPoint3 pos, FVector3 normal, Material* pMaterial)
	:BaseShape{ pos }
	, m_Normal{ GetNormalized(normal) }
	, m_pMaterial{ pMaterial }
{
}

bool Plane::Hit(const Ray& ray, HitInfo& hitInfo, bool isShadowRay) const
{
	float t = (Dot(m_Origin - ray.origin, m_Normal)) / Dot(ray.direction, m_Normal);

	if (t < ray.tMin || t> ray.tMax)
	{
		return false;
	}

	hitInfo.hitPos = ray.origin + (t)*ray.direction;
	hitInfo.hitTValue = t;
	hitInfo.hitpMaterial = m_pMaterial;
	hitInfo.hitNormal = m_Normal;

	return true;
}