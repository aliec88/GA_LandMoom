#include "StdAfx.h"
#include "Matrix2D.h"


void CMatrix2D::Matrix2DMultiply(SMatrix& m)
{
	SMatrix temp;
	//first row
	temp._11=m_sMatrix._11*m._11+m_sMatrix._12*m._21+m_sMatrix._13*m._31;
	temp._12=m_sMatrix._11*m._12+m_sMatrix._12*m._22+m_sMatrix._13*m._32;
	temp._13=m_sMatrix._11*m._13+m_sMatrix._12*m._23+m_sMatrix._13*m._33;
	//second row
	temp._21=m_sMatrix._21*m._11+m_sMatrix._22*m._21+m_sMatrix._23*m._31;
	temp._22=m_sMatrix._21*m._12+m_sMatrix._22*m._22+m_sMatrix._23*m._32;
	temp._23=m_sMatrix._21*m._13+m_sMatrix._22*m._23+m_sMatrix._23*m._33;
	//third row
	temp._31=m_sMatrix._31*m._11+m_sMatrix._32*m._21+m_sMatrix._33*m._31;
	temp._32=m_sMatrix._31*m._12+m_sMatrix._32*m._22+m_sMatrix._33*m._32;
	temp._33=m_sMatrix._31*m._13+m_sMatrix._32*m._23+m_sMatrix._33*m._33;

	m_sMatrix=temp;
}

CMatrix2D::CMatrix2D(void)
{
	Identity();
}


CMatrix2D::~CMatrix2D(void)
{
}

void CMatrix2D::Identity()
{
	m_sMatrix._11=1.0f;m_sMatrix._12=0;m_sMatrix._13=0;
	m_sMatrix._21=0;m_sMatrix._22=1.0f;m_sMatrix._23=0;
	m_sMatrix._31=0;m_sMatrix._32=0;m_sMatrix._33=1.0f;
}

void CMatrix2D::Translate(double x,double y)
{
	SMatrix model;
	model._11=1.0;model._12=0;    model._13=0;
	model._12=0;  model._22=1.0f; model._23=0;
	model._31=x;  model._32=y;    model._33=1.0f;
	Matrix2DMultiply(model);
}

void CMatrix2D::Scale(double x,double y)
{
	SMatrix model;
	model._11=x  ;model._12=0;    model._13=0;
	model._12=0;  model._22=y;    model._23=0;
	model._31=0;  model._32=0;    model._33=1.0f;
	Matrix2DMultiply(model);
}

void CMatrix2D::Rotate(double r)
{
	SMatrix model;
	model._11=cos(r)  ; model._12=sin(r);    model._13=0;
	model._21=-sin(r);  model._22=cos(r);    model._23=0;
	model._31=0;        model._32=0;         model._33=1.0f;
	Matrix2DMultiply(model);
}

void CMatrix2D::ApplyToShape(vector<SPoint>& shape)
{
	for (int i=0;i<shape.size();i++)
	{
		double x=shape[i].x*m_sMatrix._11+shape[i].y*m_sMatrix._21+1*m_sMatrix._31;
		double y=shape[i].x*m_sMatrix._12+shape[i].y*m_sMatrix._22+1*m_sMatrix._32;
		shape[i].x=x;
		shape[i].y=y;
	}
}
