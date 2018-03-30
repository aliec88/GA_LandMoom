#pragma once
struct SPoint
{
	double x,y;
	SPoint(){x=0;y=0;}
	SPoint(double a,double b)
	{
		x=a;y=b;
	}

};
struct SVector2D
{
	double x,y;
	SVector2D()
	{
		x=0;
		y=0;
	}
	SVector2D(double a,double b)
	{
		x=a;
		y=b;
	}
	SVector2D& operator+=(const SVector2D& r)
	{
		x+=r.x;
		y+=r.y;
		return *this;
	}
};