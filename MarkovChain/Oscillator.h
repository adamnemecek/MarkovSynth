#pragma once
#include "pluginconstants.h"
#include "synthfunctions.h"
#include "ModulationMatrix.h"


#define OSC_FO_MOD_RANGE 2			//2 semitone default
#define OSC_HARD_SYNC_RATIO_RANGE 4	//4
#define OSC_PITCHBEND_MOD_RANGE 12	//12 semitone default
#define OSC_FC_MIN 20				//20Hz
#define OSC_FC_MAX 20480			//20.480kHz = 10 octaves up from 20Hz
#define OSC_PULSEWIDTH_MIN 2		//2%
#define OSC_PULSEWIDTH_MAX 98		//98%

class COscillator
{
public:
	COscillator(void);
	virtual ~COscillator(void);	

	// --- mod matrix variables
	// the matrix
	CModulationMatrix *m_pModulationMatrix;
	// source and destination array index values
	UINT m_uModSourceFo; 
	UINT m_uModSourcePulseWidth; 
	UINT m_uModSourceAmp;
	UINT m_uModDestOutput1;
	UINT m_uModDestOutput2;
	
	// --- user controls or MIDI 
	double m_dOscFo;		// oscillator frequency from MIDI note number
	double m_dFoRatio;	    // FM Synth Modulator OR Hard Sync ratio 
	double m_dAmplitude;	// 0->1 from GUI

	double m_dModulo;		// modulo counter 0->1
	double m_dInc;			// phase inc = fo/fs

	int m_nOctave;			// octave tweak
	int m_nSemitones;		// semitones tweak
	int m_nCents;			// cents tweak
	
	double m_dPulseWidthControl;	// pulse width in % (sqr only) from GUI
	
	// for PITCHED Oscillators
	enum {SINE,SAW1,SAW2,SAW3,TRI,SQUARE,NOISE,PNOISE};
	UINT m_uWaveform;	// to store type
	
	// for LFOs
	enum {sine,usaw,dsaw,tri,square,expo,rsh,qrsh};

	// for LFOs - MODE
	enum {sync,shot,free};
	UINT m_uLFOMode;	// to store MODE
	
	// for hard sync or other dual-oscillator ideas
	COscillator* m_pBuddyOscillator;

	// flag indicating we are a master oscillator
	bool m_bMasterOsc;
	
	// MIDI note that is being played
	UINT m_uMIDINoteNumber;
	
protected:
	// --- calculation variables
	double m_dSampleRate;	// fs
	double m_dFo;			// current Fo	
	double m_dPulseWidth;	// pulse width in % for calculation
	bool   m_bSquareEdgeRising; // hysteresis for square edge
	
	// for noise and random sample/hold
	UINT   m_uPNRegister;	// for PN Noise sequence
	int    m_nRSHCounter;	// random sample/hold counter
	double m_dRSHValue;		// currnet rsh output
	
	// for DPW
	double m_dDPWSquareModulator;	// square toggle for DPW
	
	// memory registers for differentiator for DPW
	double m_dDPW_z1;
	
	// for modulation of oscillator pitch
	double m_dFoMod;			/* modulation input -1 to +1 */
	double m_dPitchBendMod;	    /* modulation input -1 to +1 */
	double m_dFoModLin;			/* FM Synth modulation input -1 to +1 */

	// for PWM
	double m_dPWMod;		/* modulation input for PWM -1 to +1 */

	// modulator on the amp
	double m_dAmpMod;
		
	// continuous phase modulation
	double m_dPhaseMod;

	// oscillator run flag
	bool m_bNoteOn;
	

public:
	// --- modulo functions for master/slave operation
	// increment the modulo counters
	inline void incModulo(){m_dModulo += m_dInc;}

	// true if modulo wrapped
	inline bool checkWrapModulo()
	{	
		// for positive frequencies
		if(m_dInc > 0 && m_dModulo >= 1.0) 
		{
			m_dModulo -= 1.0; 
			return true;
		}

		// for negative frequencies
		if(m_dInc < 0 && m_dModulo <= 0.0) 
		{
			m_dModulo += 1.0; 
			return true;
		}
		return false;
	}
	
	// reset the modulo (required for master->slave operations)
	inline void resetModulo(double d = 0.0){m_dModulo = d;}

	// --- modulation functions - not needed/used if you implement the modMatrix!
	// output amplitude modulation; 0->1 gets converted to dB
	inline void setAmplitudeMod(double dAmp){m_dAmpMod = dAmp;}

	// modulation, exponential
	inline void setFoModExp(double dMod){m_dFoMod = dMod;}
	inline void setPitchBendMod(double dMod){m_dPitchBendMod = dMod;}
	
	// for FM synthesis only
	inline void setFoModLin(double dMod){m_dFoModLin = dMod;}

	// for PM synthesis only
	inline void setPhaseMod(double dMod){m_dPhaseMod = dMod;}

	// for square waves only
	inline void setPWMod(double dMod){m_dPWMod = dMod;}

	// --- virtual overrides
	// sample rate
	virtual void setSampleRate(double dFs){m_dSampleRate = dFs;}
	
	// reset counters, etc...
	virtual void reset();

	// start/stop control
	virtual void startOscillator() = 0;
	virtual void stopOscillator() = 0;

	// render a sample
	// for LFO:		pAuxOutput = QuadPhaseOutput
	//     Pitched: pAuxOutput = Right channel (return value is left Channel
	//virtual double doOscillate(); // pure abstract

	// recalculate due to change in fo
	inline virtual void updateOscillator()
	{
		// ignore LFO mode for noise sources
		if(m_uWaveform == rsh || m_uWaveform == qrsh)
		   m_uLFOMode = free;
		
		// --- Modulation --- //
		// 
		// get from matrix Sources
//		if(m_pModulationMatrix)
//		{
//			// zero is norm for these
//			m_dFoMod = m_pModulationMatrix->m_dDestinations[m_uModSourceFo];
//			m_dPWMod = m_pModulationMatrix->m_dDestinations[m_uModSourcePulseWidth];
//
//			// amp mod is 0->1
//			if(m_uModSourceAmp != DEST_NONE) // can not be zero
//				m_dAmpMod = m_pModulationMatrix->m_dDestinations[m_uModSourceAmp];
//
//		}

		// do the frequency mod
		m_dFo = m_dOscFo*m_dFoRatio*pitchShiftMultiplier(m_dFoMod + 
													     m_dPitchBendMod + 
													     m_nOctave*12.0 + 
													     m_nSemitones + 
													     m_nCents/100.0);
		// apply linear FM
		m_dFo += m_dFoModLin;

		// +/- 20480 for FM/PM
		if(m_dFo > OSC_FC_MAX)
			m_dFo = OSC_FC_MAX;
		if(m_dFo < -OSC_FC_MAX)
			m_dFo = -OSC_FC_MAX;

		// calculate increment
		m_dInc = m_dFo/m_dSampleRate;

		// --- Pulse Width Modulation --- //
		// limits are 2% and 98%
		m_dPulseWidth = m_dPulseWidthControl + m_dPWMod*(OSC_PULSEWIDTH_MAX - OSC_PULSEWIDTH_MIN)/OSC_PULSEWIDTH_MIN; 

		m_dPulseWidth = fmin(m_dPulseWidth, (double)OSC_PULSEWIDTH_MAX);
		m_dPulseWidth = fmax(m_dPulseWidth, (double)OSC_PULSEWIDTH_MIN);
	}

};
