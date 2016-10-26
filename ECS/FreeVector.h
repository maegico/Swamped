#pragma once
#include <vector>

using namespace std;

//A vector wrapper for easy index reuse
template <typename T>
class FreeVector {
public:
	FreeVector() {

	}
	~FreeVector() {
		//m_vector.clear();
		//m_freeIndices.clear();
	}

	//Adds the given item at the lowest available index
	int add(T item) {
		//if there are unused indices
		if (m_freeIndices.size() > 0)
		{
			//add at the first unused index
			int index = m_freeIndices[0];
			m_vector[index] = item;
			//then remove the index
			m_freeIndices.erase(m_freeIndices.begin());
			return index;
		}
		//if there are no unused indices
		else
		{
			//add at the end of the vector
			m_vector.push_back(item);
			return m_vector.size() - 1;
		}
	}

	//Marks an index as available
	void free(unsigned int index) {
		if (m_vector.size() > index)
			m_freeIndices.push_back(index);
	}

	//Gets the size of the underlying vector
	unsigned int size() {
		return m_vector.size();
	}

	T& operator[] (const int index) {
		return m_vector[index];
	}
private:
	//Data store
	vector<T> m_vector;
	//List of available indices before the end of the vector
	vector<int> m_freeIndices;
};