#ifndef __RANDGEN_H__
#define __RANDGEN_H__

#include "Globals.h"
#include "Math.h"

class RandGen
{
public:
	RandGen();
	RandGen(uint32 seed);
	virtual ~RandGen();

	uint32 GetRandInt();
	int GetRandInt(int min, int max);
	float GetRandFloat();
	float GetRandFloat(float min, float max);

private:
	LCG* lcg = nullptr;
};


#endif // !__RANDGEN_H__