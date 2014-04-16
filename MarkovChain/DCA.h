#pragma once
#include "synthfunctions.h"
#include "ModulationMatrix.h"

#define AMP_MOD_RANGE -96	// -96dB

class CDCA
{
public:
	CDCA(void);
	~CDCA(void);
	
	CModulationMatrix* m_pModulationMatrix;
	
	UINT m_uModSourceEG;
	UINT m_uModSourceAmp;
	UINT m_uModSourceVelocity;
	UINT m_uModSourcePan;

protected:
	// our internal gain variable
	double m_dGain;				

	// velocity input from MIDI keyboard
	// need this separated for legato mode NO figure it out!
	UINT m_uMIDIVelocity; // 0 -> 127
	
	// controls for user GUI (optional)
	double m_dAmplitude_dB;		// the user's control setting in dB
	double m_dAmplitudeControl;	// the user's control setting, converted from dB
	
	// pan control
	double m_dPanControl;	/* -1 to +1 == left to right */

	// modulate amplitude
	double m_dAmpMod;				/* modulation input for LFOs */

	// input to EGMod is EXPONENTIAL
	double m_dEGMod;		 /* modulation input for EG 0 to +1 */

	// input to modulate pan control is bipolar
	double m_dPanMod;			/* modulation input for EG 0 to +1 */

public:
	// MIDI controller functions
	inline void setMIDIVelocity(UINT u){m_uMIDIVelocity = u;}

	// expecting connection from bipolar source (LFO)
	// but this component will only be able to attenuate
	// so convert to unipolar 
	inline void setAmpMod(double d){m_dAmpMod = bipolarToUnipolar(d);}

	// EG Mod Input Functions
	inline void setEGMod(double d){m_dEGMod = d;}
	
	// Pan control and Mod Input Functions
	inline void setPanControl(double d){m_dPanControl = d;}
	inline void setPanMod(double d){m_dPanMod = d;}

	inline void reset()
	{
		m_dEGMod = 1.0;
		m_dAmpMod = 1.0;
	}

	// -96dB to +20dB
	inline void setAmplitude_dB(double d)
	{
		m_dAmplitude_dB = d;
		m_dAmplitudeControl = pow((double)10.0, m_dAmplitude_dB/(double)20.0);
	}

	// recalculate gain value
	inline void updateDCA()
	{
		if(m_pModulationMatrix)
		{
			if(m_uModSourceEG != DEST_NONE)
				m_dEGMod = m_pModulationMatrix->m_dDestinations[m_uModSourceEG];
			if(m_uModSourceAmp != DEST_NONE)
				m_dAmpMod = m_pModulationMatrix->m_dDestinations[m_uModSourceAmp];
			if(m_uModSourceVelocity != DEST_NONE)
				m_uMIDIVelocity = m_pModulationMatrix->m_dDestinations[m_uModSourceVelocity];
		}
		
		// check polarity
		if(m_dEGMod >= 0)
			m_dGain = m_dEGMod;
		else
			m_dGain = m_dEGMod + 1.0;

		// amp mod is attenuation only, in dB
		m_dGain *= pow(10.0, m_dAmpMod/(double)20.0);

		// use MMA MIDI->Atten (convex) transform
		m_dGain *= mmaMIDItoAtten(m_uMIDIVelocity);	
	}

	// do the DCA: uses pass-by-reference for outputs
	inline void doMonoDCA(double dInput, double& dLeftOutput, double& dRightOutput)
	{
		if(m_pModulationMatrix)
		{
			if(m_uModSourcePan != DEST_NONE)
				m_dPanMod = m_pModulationMatrix->m_dDestinations[m_uModSourcePan];
		}

		// total pan value	
		double dPanTotal = m_dPanControl + m_dPanMod;
		
		// limit in case pan control is biased
		dPanTotal = fmin(dPanTotal, 1.0);
		dPanTotal = fmax(dPanTotal, -1.0);
        
		double dPanLeft = 0.707;
		double dPanRight = 0.707;

		// equal power calculation in synthfunction.h
		calculatePanValues(dPanTotal, dPanLeft, dPanRight);

		// form left and right outputs
		dLeftOutput =  dPanLeft*m_dAmplitudeControl*dInput*m_dGain;
		dRightOutput =  dPanRight*m_dAmplitudeControl*dInput*m_dGain;
	}

	// do the DCA: uses pass-by-reference for outputs
	// stereo version
	inline void doStereoDCA(double dLeftInput, double dRightInput, double& dLeftOutput, double& dRightOutput)
	{
		// total pan value	
		if(m_pModulationMatrix)
		{
			if(m_uModSourcePan != DEST_NONE)
				m_dPanMod = m_pModulationMatrix->m_dDestinations[m_uModSourcePan];
		}

		// total pan value	
		double dPanTotal = m_dPanControl + m_dPanMod;
		
		// limit in case pan control is biased
		dPanTotal = fmin(dPanTotal, 1.0);
		dPanTotal = fmax(dPanTotal, -1.0);

		double dPanLeft = 0.707;
		double dPanRight = 0.707;

		// equal power calculation in synthfunction.h
		calculatePanValues(dPanTotal, dPanLeft, dPanRight);

		// form left and right outputs
		dLeftOutput =  dPanLeft*m_dAmplitudeControl*dLeftInput*m_dGain;
		dRightOutput =  dPanRight*m_dAmplitudeControl*dRightInput*m_dGain;
	}
};
