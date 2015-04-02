#pragma once

#include "voice.h"
#include "AlgorithmicOscillator.h"
#include "MoogLadderFilter.h"

class CMiniSynthVoice : public CVoice
{
public:
	CMiniSynthVoice(void);
	~CMiniSynthVoice(void);

	// our synth components: Oscillators and Filters
	// Four oscillators
    CAlgorithmicOscillator m_Osc1;
    CAlgorithmicOscillator m_Osc2;
	
	// 1 filter
	CMoogLadderFilter m_LPF;

	// hard sync is property of VOICE!
	double m_dHSRatio;

	// --- variables SPECIFIC to this voice architecture
	double m_dLFO1_OscModIntensity;
	double m_dLFO1_Filter1ModIntensity;
	double m_dLFO1_Filter2ModIntensity;
		
	double m_dLFO1_DCAAmpModIntensity;

	double m_dEG1_OscModIntensity;
	double m_dEG1_Filter1ModIntensity;
	double m_dEG1_Filter2ModIntensity;
	double m_dEG1_DCAAmpModIntensity;
	double m_dLFO1_DCAPanModIntensity;

	float m_fOsc1Level;
	float m_fOsc2Level;

	//Distortion Stuff - Taken from ArcTanWaveShaper from Last Year;
	float m_fArcTanKPos;
	float m_fArcTanKNeg;
	int m_nStages;
	UINT m_uInvertStages;
	float m_fGain;
    
	UINT m_uModMode;
	
    
    
    
    // ------------------------------------------------ //
	
	// waveshape modes
	enum {SawX3,SqrX3,SawSqrSaw,TriSawTri,TriSqrTri,HSSaw};
	enum {NONE,RING,SYNC,RINGSYNC};

public:
	// intensity controls from UI -- these are all VOICE SPECIFIC! 
	inline void setOsc1Waveform(UINT uWF){m_Osc1.m_uWaveform = uWF;}
	inline void setOsc2Waveform(UINT uWF){m_Osc2.m_uWaveform = uWF;}
    
	inline void setOsc1Level(float m_fLevel){m_fOsc1Level = m_fLevel/100.0;}
	inline void setOsc2Level(float m_fLevel){m_fOsc2Level = m_fLevel/100.0;}
    
	inline void setOscDetuneSemitones(int semitones){m_Osc2.m_nSemitones = semitones;}
    
	inline void setModMode(UINT modmode){m_uModMode = modmode;}
    
	inline void setOscEGIntensity(double dIntensity){m_dEG1_OscModIntensity = dIntensity;}
	inline void setDCAEGIntensity(double dIntensity){m_dEG1_DCAAmpModIntensity = dIntensity;}
    
	inline void setOscLFOIntensity(double dIntensity){m_dLFO1_OscModIntensity = dIntensity;}
    
	inline void setDCAPanLFOIntensity(double dIntensity){m_dLFO1_DCAPanModIntensity = dIntensity;}
    
	inline void setFilter1EGIntensity(double dIntensity){m_dEG1_Filter1ModIntensity = dIntensity;}
	inline void setFilter2EGIntensity(double dIntensity){m_dEG1_Filter2ModIntensity = dIntensity;}
    
	inline void setFilter1LFOIntensity(double dIntensity){m_dLFO1_Filter1ModIntensity = dIntensity;}
	inline void setFilter2LFOIntensity(double dIntensity){m_dLFO1_Filter2ModIntensity = dIntensity;}
    
	inline void setLFO1DCAAmpModIntensity(double dIntensity){m_dLFO1_DCAAmpModIntensity = dIntensity;}
    
	inline void setHSRatio(double d){m_dHSRatio = d;}
    
	inline void setArcTanKPos(float fArcTanKPos){m_fArcTanKPos = fArcTanKPos;}
	inline void setArcTanKNeg(float fArcTanKNeg){m_fArcTanKNeg = fArcTanKNeg;}
    
	inline void setWaveshapeStages(int nStages){m_nStages = nStages;}
    
	inline void setInvertStages(UINT uInvertStages){m_uInvertStages = uInvertStages;}
    
	inline void setGain(float fGain){m_fGain = fGain;}
	
	inline virtual void setDetuneCents(double d)
	{
		// 3 ocsillators, detune two of them leave center pitch
		m_Osc1.m_nCents = d;
		m_Osc2.m_nCents = -d;
		
		// leave m_Osc3 alone
	}



	// --- Overrides
	virtual void prepareForPlay();
	virtual void setSampleRate(double dSampleRate);
	virtual void update();
	virtual void reset();

	inline virtual bool doVoice(double& dLeftOutput, double& dRightOutput)
	{
		// this does basic on/off work zero impact on speed
		if(!CVoice::doVoice(dLeftOutput, dRightOutput))
			return false;

		// --- ARTICULATION BLOCK --- //
		// layer 1 modulators
		double env1 = m_EG1.doEnvelope();
	
		
		// update for attack/decay mods
		m_LPF.updateFilter();

		// update oscillators
		m_Osc1.updateOscillator();
		m_Osc2.updateOscillator();

		// --- DIGITAL AUDIO ENGINE BLOCK --- //
		double dOscMix = 0.0;
		if(m_uModMode == NONE)
			dOscMix = m_fOsc1Level*m_Osc1.doOscillate()*env1 +
                      m_fOsc2Level*m_Osc2.doOscillate()*env1;
		else if(m_uModMode == RING)
			dOscMix = m_fOsc1Level*m_Osc1.doOscillate()*env1 +
                      m_fOsc2Level*m_Osc2.doOscillate()*env1;

		//Distortion
        dOscMix = m_LPF.doFilter(dOscMix);

//		float m_fCookedGain = pow(10.0, m_fGain/20.0);
//		dOscMix *= m_fCookedGain;
//		for(int i=0; i<m_nStages; i++)
//		{
//			if(dOscMix <=0)
//				dOscMix = (1.0/atan(m_fArcTanKPos))*atan(m_fArcTanKPos*dOscMix);
//			else
//				dOscMix = (1.0/atan(m_fArcTanKNeg))*atan(m_fArcTanKNeg*dOscMix);
//	
//			if(m_uInvertStages == ON && i%2 ==0)
//				dOscMix *= -1.0;
//		}
        dLeftOutput = dOscMix;
        dRightOutput = dOscMix;

		return true;
	}
};
