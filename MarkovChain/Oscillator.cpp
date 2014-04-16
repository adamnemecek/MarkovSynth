#include "Oscillator.h"

COscillator::COscillator(void)
{
	m_dSampleRate = 44100;	

	m_dModulo = 0.0;		
	m_dInc = 0.0;			
	m_dFo = 440.0;			
	m_dOscFo = 440.0; // GUI
	m_dAmplitude = 1.0; // default ON

	m_bSquareEdgeRising = false;
	m_dPulseWidth = 50.0;	
	m_dPulseWidthControl = 50.0; // GUI

	srand(time(NULL));
	m_uPNRegister = rand();
	m_nRSHCounter = -1; // flag for reset condition
	m_dRSHValue = 0.0;

	m_dAmpMod = 1.0;
	m_dFoModLin = 0.0;
	m_dFoMod = 0.0;
	m_dPitchBendMod = 0.0;
	m_dPWMod = 0.0;

	m_nOctave = 0.0;
	m_nSemitones = 0.0;
	m_nCents = 0.0;
	m_dFoRatio = 1.0;

	m_dPhaseMod = 0.0;
	m_uWaveform = SINE;

	m_pBuddyOscillator = NULL;
	m_bMasterOsc = false;

//	m_pModulationMatrix = NULL;

	m_uModSourceFo = DEST_NONE;
	m_uModSourcePulseWidth = DEST_NONE;
	m_uModDestOutput1 = SOURCE_NONE;
	m_uModDestOutput2 = SOURCE_NONE;
	m_uModSourceAmp = DEST_NONE;

	m_bNoteOn = false;
	m_uMIDINoteNumber = 0;
}

COscillator::~COscillator(void)
{
}
	
// virtual functions; base class implementations
void COscillator::reset()
{
	// Pitched modulos - wavetables start at 0.0
	m_dModulo = 0.0;	
		
	// needed fror triangle algorithm, DPW
	m_dDPWSquareModulator = -1.0;

	// flush DPW registers
	m_dDPW_z1 = 0.0;

	// for random stuff
	srand(time(NULL));
	m_uPNRegister = rand();
	m_nRSHCounter = -1; // flag for reset condition
	m_dRSHValue = 0.0;

	// square state variable
	m_bSquareEdgeRising = false;

	// modulation variables
	m_dPWMod = 0.0;
	m_dPitchBendMod = 0.0;
	m_dFoMod = 0.0;
	m_dAmpMod = 1.0;
	m_dFoModLin = 0.0;
	m_dPhaseMod = 0.0;
}
