#pragma once
#include "EMath.h"
#include "Ray.h"
#include "HitInfo.h"
#include "ERGBColor.h"
using namespace Elite;
class BaseShape
{
public:
	virtual ~BaseShape();
	virtual bool Hit(const Ray& ray, HitInfo& hitRecord, bool isShadowRay) const = 0;
	BaseShape(const FPoint3& center);
protected:
	FPoint3 m_Origin;
};
