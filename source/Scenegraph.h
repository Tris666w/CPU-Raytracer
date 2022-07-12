#pragma once
#include <vector>
#include "BaseShape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Ray.h"
#include "HitInfo.h"
#include "Triangle.h"
#include "TriangleMesh.h"
class Scenegraph final
{
private:
	static Scenegraph m_pInstance;
	~Scenegraph();
	Scenegraph();

	Scenegraph(const Scenegraph&) = delete;
	Scenegraph& operator=(const Scenegraph&) = delete;

	std::vector<std::vector<BaseShape*>> m_pShapesVectorVector{};

public:
	static Scenegraph& getInstance();
	void AddObjectToGraph(Sphere* newpSphere, int sceneIdx);
	void AddObjectToGraph(Plane* newpPlane, int sceneIdx);
	void AddObjectToGraph(Triangle* newpTriangle, int sceneIdx);
	void AddObjectToGraph(TriangleMesh* newpTriangleMesh, int sceneIdx);

	int AddScene();
	int GetAmountOfScenes()const { return m_pShapesVectorVector.size(); }

	void UpdateTriangles(int sceneIdx);
	void UpdateTriangleMeshes(int sceneIdx);

	void RotateTriangles(const FMatrix4& rotationMatrix, int sceneIdx);
	std::vector<BaseShape*> GetObjectVector(int sceneIdx)const;
};
