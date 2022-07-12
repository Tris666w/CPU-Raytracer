#include "BaseShape.h"

BaseShape::~BaseShape()
{
}

BaseShape::BaseShape(const FPoint3& pos)
	:m_Origin{ pos }
{
}