#include "Triangle.h"

Triangle::~Triangle()
{
	if (m_pMaterial != nullptr)
	{
		delete m_pMaterial;
	}
}

Triangle::Triangle(const std::vector<FPoint3>& relativePosVect, Material* pMaterial, Culling cullingMode)
	:BaseShape(FPoint3{})
	, m_RelativePosVector(relativePosVect)
	, m_pMaterial{ pMaterial }
	, m_Culling{ cullingMode }
	, m_TransformedPosVector{ relativePosVect }
	, m_Normal{ FVector3() }
	, m_HasNormal{ false }

{
}

Triangle::Triangle(const std::vector<FPoint3>& relativePosVect, Culling cullingMode)
	:BaseShape(FPoint3{})
	, m_RelativePosVector(relativePosVect)
	, m_pMaterial{ nullptr }
	, m_Culling{ cullingMode }
	, m_TransformedPosVector{ relativePosVect }
	, m_Normal{ FVector3() }
	, m_HasNormal{ false }
{
}

Triangle::Triangle(const std::vector<FPoint3>& relativePosVect, Culling cullingMode, const FVector3& normal)
	:BaseShape(FPoint3{})
	, m_RelativePosVector(relativePosVect)
	, m_Culling{ cullingMode }
	, m_TransformedPosVector{ relativePosVect }
	, m_pMaterial{ NULL }
	, m_Normal{ normal }
	, m_HasNormal{ true }
{
}

bool Triangle::Hit(const Ray& ray, HitInfo& hitInfo, bool isShadowRay) const
{
	FVector3 normal;

	if (m_HasNormal)
	{
		normal = m_Normal;
	}
	else
	{
		FVector3 a = m_TransformedPosVector[1] - m_TransformedPosVector[0];
		FVector3 b = m_TransformedPosVector[2] - m_TransformedPosVector[0];

		normal = GetNormalized(Cross(a, b));
	}

	//Culling
	if (isShadowRay)
	{
		switch (m_Culling)
		{
		case Triangle::Culling::FrontFace:

			if (Dot(normal, ray.direction) > 0)
			{
				return false;
			}
			break;
		case Triangle::Culling::BackFace:
			if (Dot(normal, ray.direction) < 0)
			{
				return false;
			}
			break;
		}
	}
	else
	{
		switch (m_Culling)
		{
		case Triangle::Culling::FrontFace:

			if (Dot(normal, ray.direction) < 0)
			{
				return false;
			}
			break;
		case Triangle::Culling::BackFace:
			if (Dot(normal, ray.direction) > 0)
			{
				return false;
			}
			break;
		}
	}

	if (Dot(normal, ray.direction) == 0)
	{
		return false;
	}

	FPoint3 center = FPoint3(((FVector3(m_TransformedPosVector[0]) + FVector3(m_TransformedPosVector[1]) + FVector3(m_TransformedPosVector[2])) / 3));

	FVector3 L = center - ray.origin;

	float t = (Dot(L, normal) / Dot(ray.direction, normal));
	if (t < ray.tMin || t > ray.tMax)
	{
		return false;
	}

	FPoint3 p = ray.origin + t * ray.direction;

	//Edge A
	FVector3 edge = m_TransformedPosVector[1] - m_TransformedPosVector[0];
	FVector3 pointToSide = p - m_TransformedPosVector[0];
	if (Dot(normal, Cross(edge, pointToSide)) < 0)
	{
		return false;
	}

	//Edge B
	edge = m_TransformedPosVector[2] - m_TransformedPosVector[1];
	pointToSide = p - m_TransformedPosVector[1];
	if (Dot(normal, Cross(edge, pointToSide)) < 0)
	{
		return false;
	}

	//Edge C
	edge = m_TransformedPosVector[0] - m_TransformedPosVector[2];
	pointToSide = p - m_TransformedPosVector[2];
	if (Dot(normal, Cross(edge, pointToSide)) < 0)
	{
		return false;
	}

	hitInfo.hitNormal = normal;
	hitInfo.hitPos = p;
	hitInfo.hitTValue = t;
	hitInfo.hitpMaterial = m_pMaterial;
	return true;
}

void Triangle::Update()
{
	FMatrix4 finalTransformMatrix = m_Transform * m_Rotation;
	for (size_t index = 0; index < 3; ++index)
	{
		m_TransformedPosVector[index] = FPoint3(finalTransformMatrix * FPoint4(m_RelativePosVector[index]));
	}
}