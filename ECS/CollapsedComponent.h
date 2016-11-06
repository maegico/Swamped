#pragma once

template <typename T>
struct CollapsedComponent {
	T m_component;
	unsigned int m_entityId;
	unsigned int m_handle;
};
