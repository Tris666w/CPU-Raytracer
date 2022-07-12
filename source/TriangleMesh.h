#pragma once
#include <vector>
#include "BaseShape.h"
#include "Triangle.h"

class TriangleMesh final : public BaseShape
{
public:
	TriangleMesh(const std::vector<FPoint3>& vertices, const std::vector<int>& indices, const FMatrix4& transform, Triangle::Culling culling, Material* pMaterial);
	TriangleMesh(const std::vector<FPoint3>& vertices, const std::vector<int>& indices, const std::vector<FVector3>& normals, const FMatrix4& transform, Triangle::Culling culling, Material* pMaterial);

	virtual ~TriangleMesh() = default;

	TriangleMesh(const TriangleMesh&) = delete;
	TriangleMesh(TriangleMesh&&) = delete;
	TriangleMesh& operator=(const TriangleMesh&) = delete;
	TriangleMesh& operator=(TriangleMesh&&) = delete;

	bool Hit(const Ray& ray, HitInfo& hitInfo, bool isShadowRay)const override;

	const FMatrix4& GetTransformation() const { return m_Transformation; };
	void SetTransformMatrix(const FMatrix4& transform) { m_Transformation = transform; };
	void SetRotationMatrix(const FMatrix4& rotation) { m_Rotation = rotation; };

	void Update();
private:
	std::vector<FPoint3> m_Vertices{};
	std::vector<int> m_Indices{};
	std::vector<FPoint3> m_TransformedVertices{};
	std::vector<FVector3> m_Normals{};
	bool m_HasNormals;
	FMatrix4 m_Rotation = FMatrix4::Identity();
	FMatrix4 m_Transformation = FMatrix4::Identity();
	Triangle::Culling m_Culling;
	Material* m_pMaterial = nullptr;
};
