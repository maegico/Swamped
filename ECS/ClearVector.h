#pragma once
#include <vector>
#include <mutex>
using namespace std;
template <typename T>
class ClearVector {
public:
	void add(T item) {
		m_mutex.lock();
		if (m_data.size() == m_count)
		{
			m_data.push_back(item);
		}
		else
			m_data[m_count] = item;

		m_count++;
		m_mutex.unlock();
	}
	void clear() {
		m_count = 0;
	}
	T& operator[](const unsigned int index) {
		if (index >= m_count)
			throw "Out of range";
		return m_data[index];
	}
	typename vector<T>::iterator begin() {
		return m_data.begin();
	}
	typename vector<T>::iterator end() {
		return m_data.end();
	}
	bool contains(T item) {
		for (auto& member : m_data)
			if (member == item)
				return true;
		return false;
	}
	void resize(size_t size) {
		m_data.resize(size);
		m_count = size;
	}
	size_t size() {
		return m_count;
	}

	ClearVector(const ClearVector<T> & other) {
		m_data = other.m_data;
		m_count = other.m_count;
	}

	ClearVector() {
		m_count = 0;
	}
private:
	vector<T> m_data;
	size_t m_count = 0;
	mutex m_mutex;
};
