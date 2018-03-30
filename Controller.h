#pragma once
class CController
{
private:
	vector<SPoint> m_vecStarVB;
	vector<SPoint>m_vecPadVB;
	vector<SPoint>m_vecPadVBTrans;
	SVector2D m_svPadPos;
	vector<CLander>m_vecLanders;
	CGa * m_pGA;
	int number;
	int m_iFitnessNum;
	bool m_bShowFitness;
	bool m_bFastRander;
	bool m_bFinished;
private:
	void TransformPad(vector<SPoint>& padpos);
public:
	CController(void);
	~CController(void);
	void Rander(HDC surface);
	bool Update();
	void ToggleShowFitness();
	void ToggleFastRander(){m_bFastRander=!m_bFastRander;}
	bool FastRander()const{return m_bFastRander;}
	bool GetFinished()const{return m_bFinished;}
	void ResetFitness();
	void Reset();
};

