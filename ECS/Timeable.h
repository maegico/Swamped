#pragma once

#include <chrono>

using namespace std::chrono;

class Timeable {
public:
	double GetTotalTime() {
		return m_totalTime;
	}
protected:
	void StartTimer() {
		m_start = high_resolution_clock::now();
	}
	void StopTimer() {
		m_totalTime += duration_cast<milliseconds>(high_resolution_clock::now() - m_start).count();
	}
private:
	double m_totalTime = 0;
	time_point<steady_clock> m_start;
};
