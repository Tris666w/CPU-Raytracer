#pragma once
#include "EMath.h"

struct Ray
{
	Ray(Elite::FPoint3 ori, Elite::FVector3 dir);
	Elite::FPoint3 origin = {};
	Elite::FVector3 direction = {};
	float tMin = 0.0001f;
	float tMax = FLT_MAX;
};
