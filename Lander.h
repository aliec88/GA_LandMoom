#pragma once
class CLander
{
private:
	vector<SPoint>m_vecShipVB;
	vector<SPoint>m_vecShipVBTran;
	vector<SPoint>m_vecJetVB;
	vector<SPoint>m_vecJetVBTran;
	vector<EAction>m_vecAction;
	SVector2D m_svPos;
	SVector2D m_svVelocity;
	SVector2D m_svPadPos;
	double    m_dRotation;
	double    m_dScale;
	double    m_dMass;
	bool      m_bJet;
	int m_iTick;
	bool m_bCheckedLanded;
	double m_dFitness;
private:
	 double distformpad;
	 double speed;
	 bool bDisplaynumber;
private:
	void Transform(vector<SPoint>& ship);
	bool TestForImpact(const vector<SPoint>& ship);
	void CaculateFitness();
public:
	CLander(const SVector2D& padpos);
	~CLander(void);
	void Decode(const vector<SGenBit>& action);
	void Rander(HDC surface);
	bool UpdateShip();
	double GetFitness(){return m_dFitness;}
	void SetDisplayNumber(bool display){bDisplaynumber=display;}
	void Reset();
};

