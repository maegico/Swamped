#pragma once
#include <vector>
using namespace std;
template <typename T>
class ClearVector {
public:
	void add(T item) {
		if (m_data.size() == m_count)
		{
			m_data.push_back(item);
		}
		else
			m_data[m_count] = item;

		m_count++;
	}
	void clear() {
		m_count = 0;
	}
	T& operator[](const int index) {
		if (index >= m_count || index < 0)
			throw "Out of range";
		return m_data[index];
	}
	void resize(size_t size) {
		m_data.resize(size);
		m_count = size;
	}
	size_t size() {
		return m_count;
	}
private:
	vector<T> m_data;
	size_t m_count = 0;
};
