#pragma once
#include "filter.h"

class CVAOnePoleFilter :
	public CFilter
{
public:
	CVAOnePoleFilter(void);
	~CVAOnePoleFilter(void);

	//Trapezoidal Integrator Components
	double m_dAlpha; //FeedForward coeff

	// -- Added for Korg35 Filter
	double m_dBeta;

	//provide access to you feedback output
	float getFeedbackOutput(){return m_dZ1*m_dBeta;}

	//--CFilter Overrides --
	virtual void reset(){m_dZ1 = 0;}
	virtual void updateFilter();
	virtual double doFilter(double xn);

protected:
	double m_dZ1; //our z-1 storage location;
};
