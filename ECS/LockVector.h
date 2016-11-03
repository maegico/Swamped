#pragma once
#include <vector>
#include <mutex>
using namespace std;
template <typename T>
class LockVector {
public:
	void push_back(T item) {
		m_mutex.lock();
		m_data.push_back(item);
		m_mutex.unlock();
	}

	size_t size() {
		return m_data.size();
	}

	void clear() {
		m_mutex.lock();
		m_data.clear();
		m_mutex.unlock();
	}
	T& operator[] (const int index) {
		return m_data[index];
	}
	LockVector() {
		m_data = vector<T>();
	}
	LockVector(LockVector const& src) {
		m_data = src.m_data;
	}
private:
	vector<T> m_data;
	mutex m_mutex;
};
