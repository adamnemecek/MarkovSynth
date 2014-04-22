#pragma once
#include "synthfunctions.h"
#include "ModulationMatrix.h"

class CEnvelopeGenerator
{
public:
	CEnvelopeGenerator(void);
	~CEnvelopeGenerator(void);
	
	CModulationMatrix* m_pModulationMatrix;

	UINT m_uModSourceEGAttackScaling;
	UINT m_uModSourceEGDecayScaling;
	UINT m_uModDestEGOutput;
	UINT m_uModDestBiasedEGOutput;
	
	// special override for sustain pedal
	bool m_bSustainOverride;
	bool m_bReleasePending;

	// analog and digital mode
	UINT m_uEGMode;
	enum {analog, digital};
	
	// special modes
	bool m_bResetToZero; // return to zero
	bool m_bLegatoMode;  // S-trigger 
	
protected:
	double m_dSampleRate;
	
	// current output
	double m_dEnvelopeOutput;

	// Coefficient, offset and TCO values
	// for each state
	double m_dAttackCoeff;
	double m_dAttackOffset;
	double m_dAttackTCO;

	double m_dDecayCoeff;
	double m_dDecayOffset;
	double m_dDecayTCO;

	double m_dReleaseCoeff;
	double m_dReleaseOffset;
	double m_dReleaseTCO;

	// ADSR times from user
	double m_dAttackTime_mSec;	// att: is a time duration
	double m_dDecayTime_mSec;	// dcy: is a time to decay 1->0
	double m_dReleaseTime_mSec;	// rel: is a time to decay 1->0
	
	// this is set internally; user normally not allowed to adjust
	double m_dShutdownTime_mSec; // shutdown is a time

	// sustain is a level, not a time
	double m_dSustainLevel;

	// for modulating attack and decay times
	double m_dAttackTimeScalar;	// for velocity -> attack time mod
	double m_dDecayTimeScalar;	// for note# -> decay time mod

	// inc values for shutdown
	double m_dIncShutdown;

	// stage variable
	UINT m_uState;
	enum {off, attack, decay, sustain, release, shutdown};

public:
	// accessor - allow owner to get our state
	inline UINT getState() {return m_uState;}

	inline double getEnvOutput(){return m_dEnvelopeOutput;}

	inline bool isActive()
	{
		if(m_uState != release && m_uState != off && !m_bReleasePending)
			return true;
		
		return false;
	}

	inline bool canNoteOff()
	{
		if(m_uState != release && m_uState != shutdown && m_uState != off && !m_bReleasePending)
			return true;
		
		return false;
	}
	
	// reset
	void reset();
	
	// imporant function to set the time constants
	void setEGMode(UINT u);
	
	// calculate both the exponential and linear inc values
	void calculateAttackTime();
	void calculateDecayTime();
	void calculateReleaseTime();

	// go to release state; reset counter
	void noteOff();

	// goto shutdown state
	void shutDown();

	// mutators
	inline void setState(UINT u){m_uState = u;}
	inline void setSampleRate(double d){m_dSampleRate = d;}

	inline void setSustainOverride(bool b)
	{
		m_bSustainOverride = b;

		if(m_bReleasePending && !m_bSustainOverride)
		{
			m_bReleasePending = false;
			noteOff();
		}

	}
	
	// user variables from GUI
	inline void setAttackTime_mSec(double d)
	{
		m_dAttackTime_mSec = d;
		calculateAttackTime();
	}
	inline void setDecayTime_mSec(double d)
	{
		m_dDecayTime_mSec = d;
		calculateDecayTime();
	}
	inline void setReleaseTime_mSec(double d)
	{
		m_dReleaseTime_mSec = d;
		calculateReleaseTime();
	}
	inline void setShutdownTime_mSec(double d)
	{
		m_dShutdownTime_mSec = d;
	}
	// sustain is a level not a time
	inline void setSustainLevel(double d)
	{
		m_dSustainLevel = d;
		
		// sustain level affects decay
		calculateDecayTime();
		
		// and release, if not in release state
		if(m_uState != release)
			calculateReleaseTime();
	}

	// reset and go to attack state
	inline void startEG()
	{
		// ignore
		if(m_bLegatoMode && m_uState != off && m_uState != release)
			return;

		// reset and go
		reset();
		m_uState = attack;
	}

	// go to off state
	inline void stopEG()
	{
		m_uState = off;
	}

	inline void prepareForPlay()
	{
		calculateAttackTime();
		calculateDecayTime();
		calculateReleaseTime();
	}

	// for vel->attack and note#->decay scaling modulation
	// NOTE: make sure this is only called ONCE during a new note event!
	inline void updateEG()
	{
		if(!m_pModulationMatrix) return;

		if(m_uModSourceEGAttackScaling != DEST_NONE && m_dAttackTimeScalar == 1.0)
		{
			m_dAttackTimeScalar = m_pModulationMatrix->m_dDestinations[m_uModSourceEGAttackScaling];
			calculateAttackTime();
		}
		if(m_uModSourceEGDecayScaling != DEST_NONE && m_dDecayTimeScalar == 1.0)
		{
			m_dDecayTimeScalar = m_pModulationMatrix->m_dDestinations[m_uModSourceEGDecayScaling];
			calculateDecayTime();
		}
	}

/* do the envelope

	double& dLinEnvOut = linear output passed by reference
	double& dExpEnvOut = exponential output passed by reference

*/
	

inline double doEnvelope()
{
	switch(m_uState)
	{
		case off:
		{
			// output is OFF
			m_dEnvelopeOutput = 0.0;
			break;
		}
		case attack:
		{
			// EXPONENTIAL output 
			m_dEnvelopeOutput = m_dAttackOffset + m_dEnvelopeOutput*m_dAttackCoeff;

			// check go to next state
			if(m_dEnvelopeOutput >= 1.0 || m_dAttackTimeScalar*m_dAttackTime_mSec <= 0.0)
			{
				m_dEnvelopeOutput = 1.0;
				m_uState = decay;	// go to next state	
				break;
			}

			break;
		}
		case decay:
		{
			// EXPONENTIAL output dictates cap voltage and state-change points
			m_dEnvelopeOutput = m_dDecayOffset + m_dEnvelopeOutput*m_dDecayCoeff;
		
			// go to next state if wrapped
			if(m_dEnvelopeOutput <= m_dSustainLevel || m_dDecayTimeScalar*m_dDecayTime_mSec <= 0.0)
			{
				m_dEnvelopeOutput = m_dSustainLevel;
				m_uState = sustain;		// go to next state
				break;
			}

			break;
		}
		case sustain:
		{
			// nothing to do, just set level
			m_dEnvelopeOutput = m_dSustainLevel;

			break;
		}
		case release:
		{
			if(m_bSustainOverride)
			{
				m_dEnvelopeOutput = m_dSustainLevel;
				break;
			}
			else
				// EXPONENTIAL output 
		//		m_dEnvelopeOutput = -0.0000001 + m_dEnvelopeOutput*m_dReleaseCoeff;
				m_dEnvelopeOutput = m_dReleaseOffset + m_dEnvelopeOutput*m_dReleaseCoeff;

//			if(m_dEnvelopeOutput*m_dReleaseCoeff < FLT_MIN_PLUS)
//				int t = 0;
		
			// go to next state if wrapped
			if(m_dEnvelopeOutput <= 0.0 || m_dReleaseTime_mSec <= 0.0)
			{
				m_dEnvelopeOutput = 0.0; 
				m_uState = off;		// go to next state
				break;
			}
			break;
		}	
		case shutdown:
		{
			if(m_bResetToZero)
			{
				// the shutdown state is just a linear taper since it is so short
				m_dEnvelopeOutput += m_dIncShutdown;

				// go to next state 
				if(m_dEnvelopeOutput <= 0)
				{
					m_uState = off;		// go to next state
					m_dEnvelopeOutput = 0.0; // reset envelope
					break;
				}
			}
			else
			{
				// we are guaranteed to be retriggered
				// just go to off state
				m_uState = off;	
			}
			break;
		}
	}

	if(m_pModulationMatrix)
	{
		m_pModulationMatrix->m_dSources[m_uModDestEGOutput] = m_dEnvelopeOutput;
		
		// add quad phase/stereo output
		m_pModulationMatrix->m_dSources[m_uModDestBiasedEGOutput] = m_dEnvelopeOutput - m_dSustainLevel;
	}

	// set the outputs
	return m_dEnvelopeOutput;
}

};