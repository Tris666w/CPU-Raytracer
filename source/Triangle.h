#pragma once
#include "BaseShape.h"
#include <vector>
class Triangle final : public BaseShape
{
public:
	enum class Culling
	{
		FrontFace,
		BackFace,
		NoCulling
	};
	~Triangle()override;
	Triangle(const std::vector<FPoint3>& relativePosVect, Material* pMaterial, Culling cullingMode);
	Triangle(const std::vector<FPoint3>& relativePosVect, Culling cullingMode);
	Triangle(const std::vector<FPoint3>& relativePosVect, Culling cullingMode, const FVector3& normal);

	bool Hit(const Ray& ray, HitInfo& hitInfo, bool isShadowRay)const override;

	const FMatrix4& GetTransformation()const { return m_Transform; };
	void SetTransformMatrix(const FMatrix4& transform) { m_Transform = transform; };
	void SetRotationMatrix(const FMatrix4& rotation) { m_Rotation = rotation; };

	void Update();
private:
	std::vector<FPoint3>m_RelativePosVector;
	std::vector<FPoint3>m_TransformedPosVector;
	FVector3 m_Normal;
	bool m_HasNormal;
	FMatrix4 m_Transform = FMatrix4::Identity();
	FMatrix4 m_Rotation = FMatrix4::Identity();
	Material* m_pMaterial;
	Culling m_Culling;
};
