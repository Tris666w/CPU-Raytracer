#pragma once
#include "EMath.h"
class PerspectiveCamera final 
{
public:
	PerspectiveCamera(const Elite::FPoint3& pos, float fovAngle, float distance = -1.f);

	Elite::FPoint3 GetPosition()const;
	float GetFOV()const;
	float GetDistance()const;
	Elite::FVector3 GetForwardVector()const;
	Elite::FMatrix4 GetONBMatrix()const;

	void TranslateCamera(float xTrans, float yTrans, float zTrans);
	void RotateXCamera(float angle);
	void RotateYCamera(float angle);

private:
	Elite::FPoint3 m_Position;
	float m_FOV;
	float m_Distance;
	Elite::FVector3 m_ForwardVector;
	Elite::FMatrix4 m_ONBMatrix;
	void RecalculateONB();

};

