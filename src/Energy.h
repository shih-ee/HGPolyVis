#pragma once
#include <utility>

template<class mytype =  double>
class LayoutEnergy
{
public:
	mytype AR = 0.0;
	mytype AD = 0.0;
	mytype PS = 0.0;
	mytype OC = 0.0;
	mytype DD = 0.0;
	mytype NS = 0.0;
	mytype TO = 0.0;

	mytype wAR = 0.0;
	mytype wAD = 0.0;
	mytype wPS = 0.0;
	mytype wOC = 0.0;
	mytype wDD = 0.0;
	mytype wNS = 0.0;
	mytype wTO = 0.0;

	mytype geometry = 0.0;
	mytype total = 0.0;
	mytype Gnorm = 0.0;	//gradient norm
};

template<class mytype = double>
class IterLayoutEnergy {
public:
	IterLayoutEnergy(int iter, LayoutEnergy<mytype> le) {
		mIter = iter;
		mLayoutEnergy = le;
	}

	int mIter;
	LayoutEnergy<mytype> mLayoutEnergy;
};