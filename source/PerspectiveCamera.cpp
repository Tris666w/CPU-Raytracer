#include "PerspectiveCamera.h"
#include "SDL.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(const Elite::FPoint3& pos, float fovAngle, float distance)
	:m_Distance{ distance }
	, m_Position{ pos }
	, m_FOV{ tanf(Elite::ToRadians(fovAngle / 2.f)) }
	, m_ForwardVector{ Elite::FVector3(0,0,1) }
{
	Elite::FVector3 right{ Elite::GetNormalized(Cross(Elite::FVector3{0,1,0},m_ForwardVector)) };
	Elite::FVector3 up{ Elite::GetNormalized(Cross(m_ForwardVector,right)) };
	m_ONBMatrix = Elite::FMatrix4{ Elite::FVector4(right),
		Elite::FVector4(up),
		Elite::FVector4(m_ForwardVector),
		Elite::FVector4(Elite::FVector3(m_Position),1.f) };
}

Elite::FPoint3 PerspectiveCamera::GetPosition() const
{
	return m_Position;
}

float PerspectiveCamera::GetFOV() const
{
	return m_FOV;
}

float PerspectiveCamera::GetDistance() const
{
	return m_Distance;
}

Elite::FVector3 PerspectiveCamera::GetForwardVector() const
{
	return m_ForwardVector;
}

Elite::FMatrix4 PerspectiveCamera::GetONBMatrix() const
{
	return m_ONBMatrix;
}

void PerspectiveCamera::TranslateCamera(float xTrans, float yTrans, float zTrans)
{
	Elite::FMatrix4 transMatrix = Elite::MakeTranslation(Elite::FVector3{ xTrans,yTrans,zTrans });

	m_Position = Elite::FPoint3(transMatrix * Elite::FPoint4(m_Position));

	RecalculateONB();
}

void PerspectiveCamera::RotateXCamera(float angle)
{
	angle = Elite::ToRadians(angle);
	Elite::FMatrix3 rotMatrix = Elite::MakeRotationX(angle);

	m_ForwardVector = rotMatrix * m_ForwardVector;

	RecalculateONB();
}

void PerspectiveCamera::RotateYCamera(float angle)
{
	angle = Elite::ToRadians(angle);
	Elite::FMatrix3 rotMatrix = Elite::MakeRotationY(angle);

	m_ForwardVector = rotMatrix * m_ForwardVector;

	RecalculateONB();
}

void PerspectiveCamera::RecalculateONB()
{
	Elite::FVector3 right{ Elite::GetNormalized(Cross(Elite::FVector3{0,1,0},m_ForwardVector)) };
	Elite::FVector3 up{ Elite::GetNormalized(Cross(m_ForwardVector,right)) };
	m_ONBMatrix = Elite::FMatrix4{
		Elite::FVector4(right),
		Elite::FVector4(up),
		Elite::FVector4(m_ForwardVector),
		Elite::FVector4(Elite::FVector3(m_Position),1.f) };
}