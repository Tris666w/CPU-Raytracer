#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const std::vector<FPoint3>& vertices, const std::vector<int>& indices, const FMatrix4& transform, Triangle::Culling culling, Material* pMaterial)
	:m_Vertices{ vertices }
	, m_Indices{ indices }
	, m_Transformation{ transform }
	, m_Culling{ culling }
	, BaseShape(FPoint3())
	, m_pMaterial{ pMaterial }
	, m_TransformedVertices{ m_Vertices }
	, m_HasNormals{ false }
{
}

TriangleMesh::TriangleMesh(const std::vector<FPoint3>& vertices, const std::vector<int>& indices, const std::vector<FVector3>& normals, const FMatrix4& transform, Triangle::Culling culling, Material* pMaterial)
	:m_Vertices{ vertices }
	, m_Indices{ indices }
	, m_Transformation{ transform }
	, m_Culling{ culling }
	, BaseShape(FPoint3())
	, m_pMaterial{ pMaterial }
	, m_TransformedVertices{ m_Vertices }
	, m_HasNormals{ false }
	, m_Normals{ normals }
{
}

bool TriangleMesh::Hit(const Ray& ray, HitInfo& hitInfo, bool isShadowRay) const
{
	HitInfo tempHitInfo{};
	bool hasHit = false;
	for (uint64_t index = 0, normalIndex = 0; index < m_Indices.size(); index += 3, ++normalIndex)
	{
		int index1{ m_Indices[index] };
		int index2{ m_Indices[index + 1] };
		int index3{ m_Indices[index + 2] };
		if (m_HasNormals)
		{
			Triangle triangle = Triangle({ m_TransformedVertices[index1],m_TransformedVertices[index2],m_TransformedVertices[index3] }, m_Culling, m_Normals[normalIndex]);
			if (triangle.Hit(ray, tempHitInfo, isShadowRay))
			{
				hasHit = true;
				if (tempHitInfo.hitTValue < hitInfo.hitTValue)
					hitInfo = tempHitInfo;
			}
		}
		else
		{
			Triangle triangle = Triangle({ m_TransformedVertices[index1],m_TransformedVertices[index2],m_TransformedVertices[index3] }, m_Culling);
			if (triangle.Hit(ray, tempHitInfo, isShadowRay))
			{
				hasHit = true;
				if (tempHitInfo.hitTValue < hitInfo.hitTValue)
					hitInfo = tempHitInfo;
			}
		}
	}
	hitInfo.hitpMaterial = m_pMaterial;
	return hasHit;
}

void TriangleMesh::Update()
{
	FMatrix4 finalTransformMatrix = m_Transformation * m_Rotation;
	for (size_t index = 0; index < m_Vertices.size(); ++index)
	{
		m_TransformedVertices[index] = FPoint3(finalTransformMatrix * FPoint4(m_Vertices[index]));
	}
}