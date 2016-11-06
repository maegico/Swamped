#pragma once
#include <vector>
#include <deque>
#include <iostream>

using namespace std;

//A vector wrapper for easy index reuse
template <typename T>
class FreeVector {
public:
	FreeVector() {
	}
	~FreeVector() {
	}

	//Adds the given item at the first available index
	unsigned int add(T item) {
		//if there are unused indices
		if (m_freeIndices.size() > 0)
		{
			//add at the first unused index
			unsigned int index = m_freeIndices[0];
			m_vector[index] = item;
			m_indexCatalog[index] = true;
			//then remove the index
			m_freeIndices.pop_front();
			m_count++;
			return index;
		}
		//if there are no unused indices
		else
		{
			//add at the end of the vector
			m_vector.push_back(item);
			m_indexCatalog.push_back(true);
			m_count++;
			return m_vector.size() - 1;
		}
	}

	//Marks an index as available
	void free(unsigned int index) {
		if (m_vector.size() > index && m_indexCatalog[index] == true)
		{
			m_freeIndices.push_back(index);
			m_indexCatalog[index] = false;
			m_count--;
		}
	}

	//Gets the size of the underlying vector
	size_t size() {
		return m_vector.size();
	}

	//Gets the number of filled slots
	size_t count() {
		return m_count;
	}

	T& operator[] (const int index) {
		return m_vector[index];
	}
private:
	//Data store
	vector<T> m_vector;

	//List of available indices before the end of the vector
	deque<int> m_freeIndices;

	vector<bool> m_indexCatalog;

	size_t m_count = 0;
};