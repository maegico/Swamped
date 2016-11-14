#pragma once
#include "EntityIdTypeDef.h"

template <typename T>
struct CollapsedComponent {
	T m_component;
	EntityId m_entityId;
	unsigned int m_handle;
};
