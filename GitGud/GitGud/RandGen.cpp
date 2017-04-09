#include "RandGen.h"


RandGen::RandGen()
{
	lcg = new math::LCG();
}

RandGen::RandGen(uint32 seed)
{
	lcg = new math::LCG(seed);
}


RandGen::~RandGen()
{
	RELEASE(lcg);
}

uint32 RandGen::GetRandInt()
{
	return lcg->Int();
}

int RandGen::GetRandInt(int min, int max)
{
	return lcg->Int(min, max);
}

float RandGen::GetRandFloat()
{
	return lcg->Float();
}

float RandGen::GetRandFloat(float min, float max)
{
	return lcg->Float(min, max);
}
