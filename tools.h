#pragma once
#include "stdafx.h"
inline int RandInt(int a,int b)
{
	return rand()%(b-a)+a;
}
inline double RandDouble()
{
	return rand()/(RAND_MAX+1.0);
}
inline wstring itos(int i)
{
	wostringstream s;
	s<<i;
	return s.str();
}
inline wstring dtos(double d)
{
	wostringstream s;
	s<<d;
	return s.str();
}
