#pragma once

#include <Windows.h>
#include <stdio.h>

class Toggle {
public:
	void Check() {
		if (GetKeyState(m_key) & 0x8000) {
			if (m_switch==false)
			{
				*m_target = !*m_target;
				m_switch = true;
			}
		}
		else {
			m_switch = false;
		}
	}
	Toggle(int key, bool* target) {
		m_key = key;
		m_target = target;
	}
private:
	int m_key;
	bool* m_target;
	bool m_switch;
};
