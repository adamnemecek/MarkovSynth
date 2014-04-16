#include "AlgorithmicOscillator.h"

CAlgorithmicOscillator::CAlgorithmicOscillator(void)
{
}

CAlgorithmicOscillator::~CAlgorithmicOscillator(void)
{
}

void CAlgorithmicOscillator::reset()
{	
	COscillator::reset();
	
	// saw/tri starts at 0.5
	if(m_uWaveform == SAW1 || m_uWaveform == SAW2 || 
	   m_uWaveform == SAW3 || m_uWaveform == TRI)
	{
		m_dModulo = 0.5;		
	}
	
	// update
	updateOscillator();
}

void CAlgorithmicOscillator::startOscillator()
{
	reset();
	m_bNoteOn = true;
}

void CAlgorithmicOscillator::stopOscillator()
{
	m_bNoteOn = false;
}
