#pragma once
#include "oscillator.h"

class CAlgorithmicOscillator : public COscillator
{
public:
	CAlgorithmicOscillator(void);
	~CAlgorithmicOscillator(void);

	// all variables are base class so nothing else do define
	
	// --- inline functions for realtime remdering stuff
	//
	// saw and hard-sync saw with polyBLEP
	inline double doSawtooth(double dModulo, double dInc, bool bReset)
	{
		double dTrivialSaw = 0.0;
		double dOut = 0.0;
		bool bDone = false;
		bool bResetPending = false;
		double dRatio = 1.0;

		// are we a master?
		if(m_bMasterOsc && m_pBuddyOscillator)
		{
			// check for reset on NEXT sample interval
			// needed for polyBLEP decision
			if(!bReset && (m_dModulo + m_dInc >= 1.0))
				bResetPending = true;

			// check and wrap the slave oscillator
			m_pBuddyOscillator->checkWrapModulo();

			// ratio is used for height calculation below
			dRatio = m_pBuddyOscillator->m_dOscFo/m_dFo;

			// reset the slave using the scaled ratio*modulo
			if(bReset)
				m_pBuddyOscillator->resetModulo(dRatio*m_dModulo);

			// store to operate on
			dModulo = m_pBuddyOscillator->m_dModulo;
		}

		// do saw shaping
		if(m_uWaveform == SAW1)			// SAW1 = normal sawtooth (ramp)
			dTrivialSaw = unipolarToBipolar(dModulo);
		else if(m_uWaveform == SAW2)	// SAW2 = one sided wave shaper
			dTrivialSaw = 2.0*(tanh(2.5*dModulo)/tanh(2.5)) - 1.0; 
		else if(m_uWaveform == SAW3)	// SAW3 = double sided wave shaper
		{
			dTrivialSaw = unipolarToBipolar(dModulo);
			dTrivialSaw = 3.0*dTrivialSaw/2.0*(1.0 - (dTrivialSaw*dTrivialSaw*dTrivialSaw*dTrivialSaw/2.3));
		}
		
		// if we are a master, the logic is more complex
		if(m_bMasterOsc && m_pBuddyOscillator)
		{
			if(bReset || bResetPending)
			{
				// height of transition
				double dHeight = 1.0;
				if(dRatio - (int)dRatio > 0)
					dHeight = dRatio - (int)dRatio;

				// use the master modulo because it is ultimately responsible for the 
				// transition location scale by height of discontinuity
				dOut = dTrivialSaw + dHeight*polyBLEP(m_dModulo, m_dInc, dHeight, false, bDone);
			}
			else // test for the normal PolyBLEP transition
				dOut = dTrivialSaw + polyBLEP(m_pBuddyOscillator->m_dModulo, 
											 m_pBuddyOscillator->m_dInc, 1.0, false, bDone); 

			// inc the buddy (slave) - NOTE: the slave's doOscillate() should NOT be called
			m_pBuddyOscillator->incModulo();

			return dOut;
		}

		// do polyBLEP as usual
		dOut = dTrivialSaw + polyBLEP(dModulo, 
										  fabs(dInc),/* fabs(dInc) is for FM synthesis with negative frequencies */
										  1.0,		/* sawtooth edge = 1.0 */
										  false,	/* falling edge */
										  bDone); 

		return dOut;
	}
		
	// square with polyBLEP
	inline double doSquare(double dModulo, double dInc, bool* pRisingEdge)
	{
		// pulse width conversion from %
		double dPW = m_dPulseWidth/100.0;

		// trivial square
		double dTrivialSquare = dModulo >= dPW ? -1.0 : +1.0;

		// setup for PolyBLEP
		bool bDone = false;

		// loc = 0 for rising edge, or pw for falling edge
		 double dEdgeLocation = *pRisingEdge ? 0.0 : dPW;

		// call the function
		double dOut = dTrivialSquare + polyBLEP(dModulo, 
												dInc, 
												1.0,	/* always 1.0 for square discont */
												*pRisingEdge, 
												bDone, 
												dEdgeLocation);

		// toggle the edge state
		if(bDone)
			*pRisingEdge = !*pRisingEdge;

		// need to correct for pulse-width
		//
		// 25% -> 0.667 amplitude(0.5/(1-0.25)), +0.333 DC Offset (1-amp)
		// 50% -> 1.0 amplitude, 0 DC offset
		// 75% -> 0.667 amplitude (0.5/0.75),    -0.333 DC Offset (1-amp)
		double dCorrection = 1.0;
		if(dPW < 0.5)
		{
			dCorrection = 0.5/(1.0 - dPW);
			dOut = dOut*dCorrection + (1.0 - dCorrection);
		}
		else if(dPW > 0.5)
		{
			dCorrection = 0.5/dPW;
			dOut = dOut*dCorrection - (1.0 - dCorrection);
		}

		return dOut;
	}

	// DPW
	inline double doTriangle(double dModulo, double dInc, double dFo, double dSquareModulator, double* pZ_register)
	{
//		double dOut = 0.0;
//		bool bDone = false;

		// bipolar conversion and squaring
		double dBipolar = unipolarToBipolar(dModulo);
		double dSq = dBipolar*dBipolar;

		// inversion
		double dInv = 1.0 - dSq;

		// modulation with square modulo
		double dSqMod = dInv*dSquareModulator;
			
		// original
		double dDifferentiatedSqMod = dSqMod - *pZ_register;
		*pZ_register = dSqMod;

		// c = fs/[4fo(1-2fo/fs)]
		double c = m_dSampleRate/(4.0*2.0*dFo*(1-dInc));

		return dDifferentiatedSqMod*c;
	}

	// -- virtual overrides
	virtual void reset();
	virtual void startOscillator();
	virtual void stopOscillator();

	// the rendering function
	virtual inline double doOscillate()
	{
		if(!m_bNoteOn)
			return 0.0;

		double dOut = 0.0;
			
		// always first
		bool bWrap = checkWrapModulo();
		
		// added for PHASE MODULATION
		double dCalcModulo = m_dModulo + m_dPhaseMod;
			checkWrapIndex(dCalcModulo);

		switch(m_uWaveform)
		{
			case SINE:
			{
				// calculate angle
				double dAngle = dCalcModulo*2.0*pi - pi;

				// call the parabolicSine approximator
				dOut = parabolicSine(-dAngle);
				
				break;
			}

			case SAW1:
			case SAW2:
			case SAW3:
			{
				// do first waveform
				dOut = doSawtooth(dCalcModulo, m_dInc, bWrap); // bWrap for master->reset operation
				
				break;
			}

			case SQUARE:
			{
				dOut = doSquare(dCalcModulo, m_dInc, &m_bSquareEdgeRising);

				break;
			}

			case TRI:
			{
				// do first waveform
				if(bWrap)
					m_dDPWSquareModulator *= -1.0;
				
				dOut = doTriangle(dCalcModulo, m_dInc, m_dFo, m_dDPWSquareModulator, &m_dDPW_z1);

				break;
			}

			case NOISE:
			{
				// use helper function
				dOut = doWhiteNoise();
				
				break;
			}

			case PNOISE:
			{
				// use helper function
				dOut = doPNSequence(m_uPNRegister);
				
				break;
			}
			default:
				break;
		}
		
		// ok to inc modulo now
		incModulo();
		if(m_uWaveform == TRI)
			incModulo();

		// m_dAmpMod is set in updateOscillator()


		// m_dAmpMod is set in updateOscillator()
		//return dOut*m_dAmplitude*m_dAmpMod;
        return dOut;
	}
};
