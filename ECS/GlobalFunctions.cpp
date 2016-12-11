#include "GlobalFunctions.h"

float fRand(float min, float max) {
	float f = (float)rand() / RAND_MAX;
	return min + f * (max - min);
}