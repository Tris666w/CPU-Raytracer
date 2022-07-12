#pragma once
#include "EMath.h"
#include "ERGBColor.h"
#include "Material.h"

using namespace Elite;
struct HitInfo
{
	FPoint3 hitPos = {};
	float hitTValue = {};
	FVector3 hitNormal = {};
	Material* hitpMaterial;
};
