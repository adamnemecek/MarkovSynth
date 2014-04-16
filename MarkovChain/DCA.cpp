#include "DCA.h"

CDCA::CDCA(void)
{
	m_dAmplitudeControl = 1.0;
	m_dGain = 1.0;
	m_dAmplitude_dB = 0.0;
	m_dEGMod = 1.0;
	m_dAmpMod = 1.0;

	m_dPanControl = 0.0;
	m_dPanMod = 0.0;

	m_uMIDIVelocity = 127;

	m_pModulationMatrix = NULL;

	// NOTE: DCA must be "wired" like other mod destinations
	m_uModSourceEG = DEST_NONE;
	m_uModSourceAmp = DEST_NONE;
	m_uModSourceVelocity = DEST_NONE;
	m_uModSourcePan = DEST_NONE;
}

CDCA::~CDCA(void)
{
}
