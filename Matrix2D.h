#pragma once
class CMatrix2D
{
private:
	struct SMatrix
	{
		double _11,_12,_13;
		double _21,_22,_23;
		double _31,_32,_33;
		SMatrix()
		{
			_11=0;_12=0;_13=0;
			_21=0;_22=0;_23=0;
			_31=0;_32=0;_33=0;
		}
	};
	SMatrix m_sMatrix;
private:
	void Matrix2DMultiply(SMatrix& m);
public:
	CMatrix2D(void);
	~CMatrix2D(void);
	void Identity();
	void Translate(double x,double y);
	void Scale(double x,double y);
	void Rotate(double r);
	void ApplyToShape(vector<SPoint>& shape);
};

