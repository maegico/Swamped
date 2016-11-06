#pragma once

template <size_t N, typename T>
class ClearArray {
public:
	void push(T item, bool prune = false) {
		if (m_count < N && !(prune && contains(item)))
		{
			m_data[m_count] = item;
			m_count++;
		}
	}
	T& operator[](const unsigned int index) {
		if (index >= m_count)
			throw "Out of range";
		return m_data[index];
	}
	void clear() {
		m_count
	}
	size_t size() {
		return m_count;
	}
	size_t capacity() {
		return N;
	}
private:
	bool contains(T item) {
		for (unsigned int c = 0; c < m_count; c++)
			if (m_data[c] == item)
				return true;
		return false;
	}
	T m_data[N];
	size_t m_count = 0;
};
