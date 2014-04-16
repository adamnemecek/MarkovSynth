#include "Filter.h"

CFilter::CFilter(void)
{
	// defaults
	m_dSampleRate = 44100;

	m_dFc = 1000.0;
	m_dFcControl = 1000.0;

	// NOTE: 10 octaves
	m_dFcMod = 0.0;
	m_dQ = 0.707;
	m_dAuxControl = 0.5;
	m_uNLP = OFF;
	m_dSaturation = 1.0;

	m_pModulationMatrix = NULL;
	m_uModSourceFc = DEST_NONE;
	m_uSourceFcControl = DEST_NONE;
}

CFilter::~CFilter(void)
{
}


// flush buffers
void CFilter::reset()
{
	// do nothing
}

// optional depending on filter type
void CFilter::setQControl(double dQControl)
{
	// do nothing
}
