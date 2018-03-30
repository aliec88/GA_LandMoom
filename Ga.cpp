#include "StdAfx.h"
#include "Ga.h"


void CGa::CreateStartPopulation()
{
	m_vecPopulation.clear();
	for(int i=0;i<POPULATION_SIZE;i++)
	{
		m_vecPopulation.push_back(SChromosome());
	}
	m_iGeneration=0;
	m_dTotalFitness=0;
	m_dBestFitness=0;
}

SChromosome& CGa::RoletteWheelSelection()
{
	double slice=RandDouble()*m_dTotalFitness;
	double total=0;
	int pos=0;
	for (int i=0;i<POPULATION_SIZE;i++)
	{
		total+=m_vecPopulation[i].m_dFitness;
		if (total>slice)
		{
			pos=i;
			break;
		}
	}
	return m_vecPopulation[pos];
}

void CGa::Mutate(vector<SGenBit>&chromosome)
{
	for(int i=0;i<CHROMOSOME_LENGHT;i++)
	{
		if (RandDouble()<MUTATION_RATE)
		{
			chromosome[i].act=(EAction)RandInt(0,4);
		}
		if (RandDouble()<MUTATION_RATE/2)
		{
			chromosome[i].duration=RandInt(1,MAX_ACTION_DURATION);
		}
	}
}

void CGa::CrossoverMulti(const vector<SGenBit>&mum,const vector<SGenBit>&dad,vector<SGenBit>&baby1,vector<SGenBit>&baby2)
{
	if(RandDouble()>CROSSOVER_RATE || mum==dad)
	{
		baby1=mum;
		baby2=dad;
		return;
	}
	double swaprate=RandDouble()*CHROMOSOME_LENGHT;
	for (int i=0;i<CHROMOSOME_LENGHT;i++)
	{
		if (RandDouble()<swaprate)
		{
			baby1.push_back(dad[i]);
			baby2.push_back(mum[i]);
		}else
		{
			baby1.push_back(mum[i]);
			baby2.push_back(dad[i]);
		}
	}
}

void CGa::Elitism(int n,vector<SChromosome>&newpop)
{
	sort(m_vecPopulation.begin(),m_vecPopulation.end());
	for (int i=0;i<n;i++)
	{
		int pos=m_vecPopulation.size()-1-i;
		newpop.push_back(m_vecPopulation[pos]);
	}
	m_dBestFitness=m_vecPopulation[POPULATION_SIZE-1].m_dFitness;
	m_dWorstFitness=m_vecPopulation[0].m_dFitness;
}

void CGa::CaculateTotalFitness()
{
	m_dTotalFitness=0;
	for (int i=0;i<POPULATION_SIZE;i++)
	{
		m_dTotalFitness+=m_vecPopulation[i].m_dFitness;
	}
}

CGa::CGa(void)
{
	CreateStartPopulation();
	m_dBestFitness=0;
	m_dTotalFitness=0;
	m_dWorstFitness=0;
	m_iGeneration=0;
}


CGa::~CGa(void)
{
}

void CGa::Epoch()
{
	vector<SChromosome>newpop;
	CaculateTotalFitness();
	Elitism(1,newpop);
	while(newpop.size()<POPULATION_SIZE)
	{
		SChromosome mum=RoletteWheelSelection();
		SChromosome dad=RoletteWheelSelection();
		SChromosome baby1,baby2;
		baby1.m_dFitness=0;
		baby1.m_vecChromsome.clear();
		baby2.m_dFitness=0;
		baby2.m_vecChromsome.clear();
		CrossoverMulti(mum.m_vecChromsome,dad.m_vecChromsome,baby1.m_vecChromsome,baby2.m_vecChromsome);
		Mutate(baby1.m_vecChromsome);
		Mutate(baby2.m_vecChromsome);
		newpop.push_back(baby1);
		newpop.push_back(baby2);
	}
	m_iGeneration++;
	m_vecPopulation.clear();
	m_vecPopulation=newpop;
}
