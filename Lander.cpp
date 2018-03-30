#include "StdAfx.h"
#include "Lander.h"

const int NumLanderVerts = 30;

const SPoint lander[NumLanderVerts] = {
	//middle of lander
	SPoint(-1, 0),
	SPoint(1, 0),    
	SPoint(1, -0.5),
	SPoint(-1, -0.5),
	//top of lander
	SPoint(-0.5, 0),
	SPoint(-1, 0.3),
	SPoint(-1, 0.7),
	SPoint(-0.5, 1),
	SPoint(0.5, 1),
	SPoint(1, 0.7),
	SPoint(1, 0.3),
	SPoint(0.5, 0),
	//legs
	SPoint(-1, -0.4),
	SPoint(-1.3, -0.8),
	SPoint(-1.3, -1.2),
	SPoint(-1.5, -1.2),
	SPoint(-1.1, -1.2),
	SPoint(-0.9, -0.5),
	SPoint(-1.3, -0.8),

	SPoint(1, -0.4),
	SPoint(1.3, -0.8),
	SPoint(1.3, -1.2),
	SPoint(1.5, -1.2),
	SPoint(1.1, -1.2),
	SPoint(0.9, -0.5),
	SPoint(1.3, -0.8),
	//rocket
	SPoint(-0.2, -0.5),
	SPoint(-0.3, -0.8),
	SPoint(0.3, -0.8),
	SPoint(0.2, -0.5)};

	//and the vertices for the jet
	const int NumJetVerts = 5;

	const SPoint jet[NumJetVerts] = {SPoint(-0.1, -0.9),
		SPoint(-0.2, -1.2),
		SPoint(0, -1.6),
		SPoint(0.2, -1.2),
		SPoint(0.1, -0.9)};

	void CLander::Transform(vector<SPoint>& ship)
	{
		CMatrix2D m;
		m.Scale(m_dScale,m_dScale);
		m.Rotate(m_dRotation);
		m.Translate(m_svPos.x,m_svPos.y);
		m.ApplyToShape(ship);
	}

	bool CLander::TestForImpact(const vector<SPoint>& ship)
	{
		for (int i=0;i<ship.size();i++)
		{
			if (ship[i].y>WINDOW_HEIGHT-100)
			{
				return true;
			}
		}
		return false;
	}

	void CLander::CaculateFitness()
	{
		 distformpad=fabs(m_svPadPos.x-m_svPos.x);
		double distfit=WINDOW_WIDTH-distformpad;
		 speed=sqrt((m_svVelocity.x*m_svVelocity.x)+
			m_svVelocity.y*m_svVelocity.y);
		double rotfit=1/(fabs(m_dRotation-PI)+1);
		double fitairtime=(double)m_iTick/(speed+1);
		m_dFitness=distfit+800*rotfit+8*fitairtime;
		if(distformpad<DIST_TOLERANCE && 
			speed<SPEED_TOLERANCE &&
			fabs(PI-m_dRotation)<ROTATION_TOLERANCE)
		{
			m_dFitness=BIGNUMBER;
		}
	}

	CLander::CLander(const SVector2D& padpos)
{
	for (int i=0;i<NumLanderVerts;i++)
	{
		m_vecShipVB.push_back(lander[i]);
	}
	m_vecShipVBTran=m_vecShipVB;
	for(int i=0;i<NumJetVerts;i++)
	{
		m_vecJetVB.push_back(jet[i]);
	}
	m_vecJetVBTran=m_vecJetVB;
	m_dRotation=PI;
	m_dScale=LANDER_SCALE;
	m_iTick=0;
	m_bCheckedLanded=false;
	m_bJet=false;
	m_dFitness=0;
	m_svPadPos=padpos;
	m_dMass=MASS;
	m_svPos=SVector2D(WINDOW_WIDTH/2,10);
	bDisplaynumber=false;
	
}


CLander::~CLander(void)
{
}

void CLander::Decode(const vector<SGenBit>& action)
{
	m_vecAction.clear();
	for (int i=0;i<action.size();i++)
	{
		for (int j=0;j<action[i].duration;j++)
		{
			m_vecAction.push_back(action[i].act);
		}
	}
}

void CLander::Rander(HDC surface)
{
	/*m_vecShipVBTran=m_vecShipVB;
	Transform(m_vecShipVBTran);*/
	MoveToEx(surface,m_vecShipVBTran[0].x,m_vecShipVBTran[0].y,NULL);
	for (int i=0;i<m_vecShipVBTran.size();i++)
	{
		LineTo(surface,m_vecShipVBTran[i].x,m_vecShipVBTran[i].y);
	}
	LineTo(surface,m_vecShipVBTran[0].x,m_vecShipVBTran[0].y);
	if (m_bJet)
	{
		m_vecJetVBTran=m_vecJetVB;
		Transform(m_vecJetVBTran);
		MoveToEx(surface,m_vecJetVBTran[0].x,m_vecJetVBTran[0].y,NULL);
		for (int i=0;i<m_vecJetVBTran.size();i++)
		{
			LineTo(surface,m_vecJetVBTran[i].x,m_vecJetVBTran[i].y);
		}
		LineTo(surface,m_vecJetVBTran[0].x,m_vecJetVBTran[0].y);
	}
	if (bDisplaynumber)
	{
		wstring strspeed=TEXT("ËÙ¶È£º")+dtos(speed);
		wstring strdis=TEXT("¾àÀë£º")+dtos(distformpad);
		wstring strrotate=TEXT("½Ç¶È£º")+dtos(m_dRotation);
		TextOut(surface,WINDOW_WIDTH-300,0,strspeed.c_str(),strspeed.size());
		TextOut(surface,WINDOW_WIDTH-300,20,strdis.c_str(),strdis.size());
		TextOut(surface,WINDOW_WIDTH-300,40,strrotate.c_str(),strrotate.size());
	}
	
}

bool CLander::UpdateShip()
{
	if (m_bCheckedLanded)
	{
		return false;
	}
	EAction currentaction;
	if (m_iTick>=m_vecAction.size())
	{
		currentaction=none;
	}else
	{
		currentaction=m_vecAction[m_iTick++];
	}
	m_bJet=false;
	switch(currentaction)
	{
	case none:
		break;
	case rotate_left:
		{
			m_dRotation-=ROTATION_PER_TICK;
			if (m_dRotation<-PI)
			{
				m_dRotation+=2*PI;
			}
		}
		break;
	case rotate_right:
		{
			m_dRotation+=ROTATION_PER_TICK;
			if (m_dRotation>2*PI)
			{
				m_dRotation-=2*PI;
			}
		}
		break;
	case thrust:
		{
			m_bJet=true;
			double acceleration=THRUST_PER_TICK/m_dMass;
			m_svVelocity.x+=acceleration*sin(m_dRotation);
			m_svVelocity.y+=acceleration*cos(m_dRotation);
		}

		break;
	}
	m_svVelocity.y+=GRAVITY_PER_TICK;
	m_svPos.x-=m_svVelocity.x;
	m_svPos.y+=m_svVelocity.y;
	if (m_svPos.x>WINDOW_WIDTH)
	{
		m_svPos.x=0;
	}
	if (m_svPos.x<0)
	{
		m_svPos.x=WINDOW_WIDTH;
	}
	
	m_vecShipVBTran=m_vecShipVB;
	Transform(m_vecShipVBTran);
	if (TestForImpact(m_vecShipVBTran))
	{
		m_bCheckedLanded=true;
		CaculateFitness();
		return false;
	}
	return true;
}

void CLander::Reset()
{
	if (UpdateShip()==false)
	{
		Sleep(2000);
		m_svPos=SVector2D(WINDOW_WIDTH/2,10);
		m_dRotation=PI;
		m_dScale=LANDER_SCALE;
		m_bCheckedLanded=false;
		m_iTick=0;
		m_svVelocity=SVector2D(0,0);
	}
	
}
