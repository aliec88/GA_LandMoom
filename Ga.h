#pragma once
enum EAction
{
	rotate_left,rotate_right,thrust,none
};
struct SGenBit
{
	EAction act;
	int duration;
	SGenBit()
	{
		act=(EAction)RandInt(0,4);
		duration=RandInt(1,MAX_ACTION_DURATION);
	}
	bool operator==(const SGenBit& r)const
	{
		return (act==r.act)&&(duration==r.duration);
	}
};
struct SChromosome
{
	vector<SGenBit>m_vecChromsome;
	double m_dFitness;
	SChromosome():m_dFitness(0)
	{
		for (int i=0;i<CHROMOSOME_LENGHT;i++)
		{
			m_vecChromsome.push_back(SGenBit());
		}
	}
	bool operator<(const SChromosome& r)
	{
		return m_dFitness<r.m_dFitness;
	}
	
};
class CGa
{
public:	
	vector<SChromosome>m_vecPopulation;
private:
	double m_dBestFitness;
	double m_dWorstFitness;
	double m_dTotalFitness;
	int m_iGeneration;
private:
	void CreateStartPopulation();
	SChromosome& RoletteWheelSelection();
	void Mutate(vector<SGenBit>&chromosome);
	void CrossoverMulti(const vector<SGenBit>&mum,const vector<SGenBit>&dad,vector<SGenBit>&baby1,vector<SGenBit>&baby2);
	void Elitism(int n,vector<SChromosome>&newpop);
	void CaculateTotalFitness();
public:
	CGa(void);
	~CGa(void);
	void Epoch();
	vector<SChromosome> GetPopulation(){return m_vecPopulation;}
	int  GetGeneration(){return m_iGeneration;}
	SChromosome schtest;
};

