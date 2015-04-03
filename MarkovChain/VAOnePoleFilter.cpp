#include "VAOnePoleFilter.h"

CVAOnePoleFilter::CVAOnePoleFilter(void)
{
	// init
	m_dAlpha = 1.0;
	m_dBeta =0.0;
	m_dZ1 = 0.0;

	// always set the default!
	m_uFilterType = LPF1;

	//flush
	reset();
}

//recalc coeffs
void CVAOnePoleFilter::updateFilter()
{
	//base class does modulation, changes m_fFc
	CFilter::updateFilter();

	double wd = 2*pi*m_dFc;
	double T = 1/m_dSampleRate;
	double wa = (2/T)*tan(wd*T/2);
	double g = wa*T/2;

	m_dAlpha = g/(1.0+g);

}

double CVAOnePoleFilter::doFilter(double xn)
{
	//return xn if filter not supported
	if(m_uFilterType != LPF1 && m_uFilterType != HPF1)
		return xn;

	//calculate v(n)
	double vn = (xn-m_dZ1)*m_dAlpha;

	//form LP output
	double lpf = vn + m_dZ1;

	//update memory
	m_dZ1 = vn + lpf;

	//do the hpf
	double hpf = xn - lpf;

	if(m_uFilterType == LPF1)
		return lpf;
	else if(m_uFilterType == HPF1)
		return hpf;

	//Always provide the default
	return xn;
}

CVAOnePoleFilter::~CVAOnePoleFilter(void)
{
}
