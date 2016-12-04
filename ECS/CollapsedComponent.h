#pragma once
#include "EntityIdTypeDef.h"

template <typename T>
struct CollapsedNonComponent {
	T m_component;
	unsigned int m_handle;
};

template <typename T>
struct CollapsedComponent{
	T m_component;
	EntityId m_entityId;
	unsigned int m_handle;
};
