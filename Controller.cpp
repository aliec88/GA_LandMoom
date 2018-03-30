#include "StdAfx.h"
#include "Controller.h"
const int NumStar=200;
const int NumPadVerts=4;
const SPoint Pad[NumPadVerts]={SPoint(-20,0),SPoint(20,0),SPoint(20,5),SPoint(-20,5)};

void CController::TransformPad(vector<SPoint>& padpos)
{
	CMatrix2D matrix;
	matrix.Translate(m_svPadPos.x,m_svPadPos.y);
	matrix.ApplyToShape(padpos);
}

CController::CController(void)
{
	for (int i=0;i<NumStar;i++)
	{
		m_vecStarVB.push_back(SPoint(RandInt(1,WINDOW_WIDTH),RandInt(1,WINDOW_HEIGHT)));
	}
	for (int i=0;i<NumPadVerts;i++)
	{
		m_vecPadVB.push_back(Pad[i]);
	}
	m_vecPadVBTrans=m_vecPadVB;
	m_svPadPos=SVector2D(RandInt(10,WINDOW_WIDTH),WINDOW_HEIGHT-100);
	m_pGA=new CGa;
	for (int i=0;i<POPULATION_SIZE;i++)
	{
		m_vecLanders.push_back(CLander(m_svPadPos));
		m_vecLanders[i].Decode(m_pGA->GetPopulation()[i].m_vecChromsome);
	}
	number=0;
	m_iFitnessNum=0;
	m_bShowFitness=false;
	m_bFastRander=false;
	m_bFinished=false;
}


CController::~CController(void)
{
	delete m_pGA;
}

void CController::Rander(HDC surface)
{
	for (int i=0;i<m_vecStarVB.size();i++)
	{
		if (RandDouble()>0.1)
		{
			SetPixel(surface,m_vecStarVB[i].x,m_vecStarVB[i].y,RGB(255,255,255));
		}
	}
	SelectObject(surface,GetStockObject(WHITE_PEN));
	m_vecPadVBTrans=m_vecPadVB;
	TransformPad(m_vecPadVBTrans);
	MoveToEx(surface,m_vecPadVBTrans[0].x , m_vecPadVBTrans[0].y,NULL);
	for (int i=0;i<m_vecPadVBTrans.size();i++)
	{
		LineTo(surface,m_vecPadVBTrans[i].x,m_vecPadVBTrans[i].y);
	}
	LineTo(surface,m_vecPadVBTrans[0].x,m_vecPadVBTrans[0].y);
	if (!m_bShowFitness)
	{
		for (int i=0;i<m_vecLanders.size();i++)
		{
			m_vecLanders[i].SetDisplayNumber(false);
			m_vecLanders[i].Rander(surface);
		}
	}else
	{
		m_vecLanders[m_iFitnessNum].SetDisplayNumber(true);
		m_vecLanders[m_iFitnessNum].Rander(surface);
	}
	
	SetBkMode(surface,TRANSPARENT);
	SetTextColor(surface,RGB(255,255,255));
	wstring s=TEXT("时代：")+itos(m_pGA->GetGeneration());
	TextOut(surface,0,0,s.c_str(),s.size());
	wstring n=TEXT("人口：")+itos(number);
	TextOut(surface,0,20,n.c_str(),n.size());
	wstring m=TEXT("F键—加速切换    B键—切换最优基因   R键—重新开始");
	TextOut(surface,WINDOW_WIDTH/3, WINDOW_HEIGHT-80,m.c_str(),m.size());
}

bool CController::Update()
{	
	static bool bAllFinishied=true;
	double bestfitness=0;
	m_iFitnessNum=0;
	number=0;
	if (bAllFinishied)
	{
		bAllFinishied=false;
		for (int i=0;i<POPULATION_SIZE;i++)
		{
			if (m_vecLanders[i].UpdateShip())
			{
				bAllFinishied=true;				
			}else
			{
				number++;
			}
		}
	}else
	{
		for (int i=0;i<POPULATION_SIZE;i++)
		{
			m_pGA->m_vecPopulation[i].m_dFitness=m_vecLanders[i].GetFitness();					
			if (bestfitness<m_vecLanders[i].GetFitness())
			{
				bestfitness=m_vecLanders[i].GetFitness();
				m_iFitnessNum=i;
				if(m_pGA->GetPopulation()[i].m_dFitness>=BIGNUMBER)
				{
					MessageBox(NULL,TEXT("已经找到最佳降落路径,点确定观看"),TEXT("成功"),0);
					m_bFinished=true;
					//ResetFitness();
					return false;
					break;
				}
			}			
		}
		m_pGA->Epoch();
		m_vecLanders.clear();
		for (int i=0;i<POPULATION_SIZE;i++)
		{
			m_vecLanders.push_back(CLander(m_svPadPos));
			m_vecLanders[i].Decode(m_pGA->GetPopulation()[i].m_vecChromsome);
		}
		bAllFinishied=true;
	}		
	m_bFinished=false;
	return true;
}

void CController::ToggleShowFitness()
{
	if (m_bShowFitness)
	{
		m_bShowFitness=false;
	}else
	{
		m_bShowFitness=true;
	}
}

void CController::ResetFitness()
{	
	m_bShowFitness=true;
	m_bFastRander=false;
//	m_bFinished=false;
	m_vecLanders[m_iFitnessNum].Reset();
}

void CController::Reset()
{
	delete m_pGA;
	m_pGA=new CGa;
	m_vecLanders.clear();
	m_svPadPos=SVector2D(RandInt(10,WINDOW_WIDTH),WINDOW_HEIGHT-100);
	for (int i=0;i<POPULATION_SIZE;i++)
	{
		m_vecLanders.push_back(CLander(m_svPadPos));
		m_vecLanders[i].Decode(m_pGA->GetPopulation()[i].m_vecChromsome);
	}
	number=0;
	m_iFitnessNum=0;
	m_bShowFitness=false;
	m_bFastRander=false;
	m_bFinished=false;
}
