#include "Scenegraph.h"
#include "BaseShape.h"

Scenegraph Scenegraph::m_pInstance{};

Scenegraph::~Scenegraph()
{
	for (size_t i = 0; i < m_pShapesVectorVector.size(); i++)
	{
		for (BaseShape* pShape : m_pShapesVectorVector[i])
		{
			delete pShape;
		}
	}
	m_pShapesVectorVector.clear();
}

Scenegraph::Scenegraph()
{
}

Scenegraph& Scenegraph::getInstance()
{
	return m_pInstance;
}

void Scenegraph::AddObjectToGraph(Sphere* newpSphere, int sceneIdx)
{
	m_pShapesVectorVector[sceneIdx].push_back(newpSphere);
}

void Scenegraph::AddObjectToGraph(Plane* newpPlane, int sceneIdx)
{
	m_pShapesVectorVector[sceneIdx].push_back(newpPlane);
}

void Scenegraph::AddObjectToGraph(Triangle* newpTriangle, int sceneIdx)
{
	m_pShapesVectorVector[sceneIdx].push_back(newpTriangle);
}

void Scenegraph::AddObjectToGraph(TriangleMesh* newpTriangleMesh, int sceneIdx)
{
	m_pShapesVectorVector[sceneIdx].push_back(newpTriangleMesh);
}

int Scenegraph::AddScene()
{
	m_pShapesVectorVector.push_back({});
	return m_pShapesVectorVector.size() - 1;
}

void Scenegraph::UpdateTriangles(int sceneIdx)
{
	Triangle* pTriangle{};
	for (BaseShape* currentShape : m_pShapesVectorVector[sceneIdx])
	{
		pTriangle = dynamic_cast<Triangle*>(currentShape);
		if (pTriangle != nullptr)
		{
			pTriangle->Update();
		}
	}
}

void Scenegraph::UpdateTriangleMeshes(int sceneIdx)
{
	TriangleMesh* pTriangleMesh{};
	for (BaseShape* currentShape : m_pShapesVectorVector[sceneIdx])
	{
		pTriangleMesh = dynamic_cast<TriangleMesh*>(currentShape);
		if (pTriangleMesh != nullptr)
		{
			pTriangleMesh->Update();
		}
	}
}

void Scenegraph::RotateTriangles(const FMatrix4& rotationMatrix, int sceneIdx)
{
	Triangle* pTriangle{};
	for (BaseShape* currentShape : m_pShapesVectorVector[sceneIdx])
	{
		pTriangle = dynamic_cast<Triangle*>(currentShape);
		if (pTriangle != nullptr)
		{
			pTriangle->SetRotationMatrix(rotationMatrix);
		}
	}
}

std::vector<BaseShape*> Scenegraph::GetObjectVector(int sceneIdx) const
{
	return m_pShapesVectorVector[sceneIdx];
}