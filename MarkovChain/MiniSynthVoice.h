#pragma once

#include "voice.h"
#include "AlgorithmicOscillator.h"
#include "MoogLadderFilter.h"

class CMiniSynthVoice : public CVoice
{
public:
	CMiniSynthVoice(void);
	~CMiniSynthVoice(void);

protected:
	// our synth components: Oscillators and Filters
	// Four oscillators
	CAlgorithmicOscillator m_Osc1;
	CAlgorithmicOscillator m_Osc2;
	CAlgorithmicOscillator m_Osc3;
	CAlgorithmicOscillator m_Osc4; // noise
	
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
    float m_fOsc3Level;
    float m_fOsc4Level;

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
    inline void setOsc3Waveform(UINT uWF){m_Osc3.m_uWaveform = uWF;}
    inline void setOsc4Waveform(UINT uWF){m_Osc4.m_uWaveform = uWF;}
	inline void setOsc1Level(float m_fLevel){m_fOsc1Level = m_fLevel/100.0;}
	inline void setOsc2Level(float m_fLevel){m_fOsc2Level = m_fLevel/100.0;}
    inline void setOsc3Level(float m_fLevel){m_fOsc3Level = m_fLevel/100.0;}
	inline void setOsc4Level(float m_fLevel){m_fOsc4Level = m_fLevel/100.0;}
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
	// EXAMPLE of how to modify the Modulation Matrix by adding or removing
	//         a SOURCE->DEST pairs
	//
	// FILTER KEY TRACK:
	//         SOURCE_MIDI_NOTE_NUM -> DEST_ALL_FILTER_KEYTRACK
	//
	inline void enableFilterKeyTrack(bool bEnable)
	{
		if(bEnable)
		{
			m_LPF.m_uSourceFcControl = DEST_FILTER1_KEYTRACK;

			// NOTE NUMBER -> FILTER Fc CONTROL
			modMatrixRow* pRow = new modMatrixRow;
			pRow->uSourceIndex = SOURCE_MIDI_NOTE_NUM;
			pRow->uDestinationIndex = DEST_ALL_FILTER_KEYTRACK;
			pRow->pModIntensity = &m_dFilterKeyTrackIntensity; // 0.5 = one octave below fc, 1 = fc, 2 = one octave above fc
			pRow->pModRange = &m_dDefaultModRange;
			pRow->uSourceTransform = TRANSFORM_NOTE_NUMBER_TO_FREQUENCY; 

			m_ModulationMatrix.addModMatrixRow(pRow);
		}
		else
		{
			m_LPF.m_uSourceFcControl = DEST_NONE;
			m_ModulationMatrix.removeMatrixRow(SOURCE_MIDI_NOTE_NUM, DEST_ALL_FILTER_KEYTRACK);
		}	
	}

	// VELOCITY TO ATTACK SCALING:
	//         SOURCE_VELOCITY -> DEST_ALL_EG_ATTACK_SCALING
	//
	inline void enableVelocityToAttackScaling(bool bEnable)
	{
		if(bEnable)
		{
			m_EG1.m_uModSourceEGAttackScaling = DEST_EG1_ATTACK_SCALING;

			// VELOCITY -> EG ATTACK SCALING
			modMatrixRow* pRow = new modMatrixRow;
			pRow->uSourceIndex = SOURCE_VELOCITY;
			pRow->uDestinationIndex = DEST_ALL_EG_ATTACK_SCALING;
			pRow->pModIntensity = &m_dDefaultModIntensity; // 1.0 for this synth
			pRow->pModRange = &m_dDefaultModRange;
			// 0 velocity -> scalar = 1, normal attack time
			// 128 velocity -> scalar = 0, fastest (0) attack time
			pRow->uSourceTransform = TRANSFORM_INVERT_MIDI_NORMALIZE; 

			m_ModulationMatrix.addModMatrixRow(pRow);
		}
		else
		{
			m_EG1.m_uModSourceEGAttackScaling = DEST_NONE;
			m_ModulationMatrix.removeMatrixRow(SOURCE_VELOCITY, DEST_ALL_EG_ATTACK_SCALING);
		}	
	}

	// MIDI NOTE # TO DECAY SCALING:
	//         SOURCE_MIDI_NOTE_NUM -> DEST_ALL_EG_DECAY_SCALING
	//
	inline void enableMIDINoteNumberToDecayScaling(bool bEnable)
	{
		if(bEnable)
		{
			m_EG1.m_uModSourceEGDecayScaling = DEST_EG1_DECAY_SCALING;

			// NOTE NUMBER -> EG DECAY SCALING
			modMatrixRow* pRow = new modMatrixRow;
			pRow->uSourceIndex = SOURCE_MIDI_NOTE_NUM;
			pRow->uDestinationIndex = DEST_ALL_EG_DECAY_SCALING;
			pRow->pModIntensity = &m_dDefaultModIntensity; // 1.0
			pRow->pModRange = &m_dDefaultModRange;
			// note#0 -> scalar = 1, normal decay time
			// note#128 -> scalar = 0, fastest (0) decay time
			pRow->uSourceTransform = TRANSFORM_INVERT_MIDI_NORMALIZE; 

			m_ModulationMatrix.addModMatrixRow(pRow);
		}
		else
		{
			m_EG1.m_uModSourceEGDecayScaling = DEST_NONE;
			m_ModulationMatrix.removeMatrixRow(SOURCE_MIDI_NOTE_NUM, DEST_ALL_EG_DECAY_SCALING);
		}	
	}

	// --- Overrides
	virtual void prepareForPlay();
	virtual void setSampleRate(double dSampleRate);
	virtual void update();
	virtual void reset();

	// --- Overriding: stuff specific to THIS synth voice
	inline void createModRow(UINT source, UINT dest, double intensity)
	{
		modMatrixRow* pRow = new modMatrixRow;
		pRow->uSourceIndex = source;
		pRow->uDestinationIndex = dest;
		pRow->pModIntensity = &intensity;
		pRow->pModRange = &m_dDefaultModRange;
		pRow->uSourceTransform = TRANSFORM_INVERT_MIDI_NORMALIZE;

		m_ModulationMatrix.addModMatrixRow(pRow);
	}
	inline void removeModRow(UINT source, UINT dest)
	{
		m_ModulationMatrix.removeMatrixRow(source, dest);
	}
	inline virtual bool doVoice(double& dLeftOutput, double& dRightOutput)
	{
		// this does basic on/off work zero impact on speed
		if(!CVoice::doVoice(dLeftOutput, dRightOutput))
			return false;

		// --- ARTICULATION BLOCK --- //
		// layer 1 modulators
		double env1 = m_EG1.doEnvelope();
//		double env2 = m_EG2.doEnvelope();
//		m_EG3.doEnvelope();
//		m_EG4.doEnvelope();

		// update and do LFO	

//		m_LFO1.updateOscillator();	
//		double lfo = m_LFO1.doOscillate();
	
		// apply modulatora
//		m_ModulationMatrix.doModulationMatrix();
		
		// update for attack/decay mods
//		m_EG1.updateEG();
//		m_EG2.updateEG();
//		m_EG3.updateEG();
//		m_EG4.updateEG();

		// update DCA and Filter
//		m_DCA.updateDCA();	
//		m_LPF.updateFilter();

		// slave is osc2 (running at higher freq)
		if(m_Osc1.m_uWaveform == SAW1 || m_Osc1.m_uWaveform == SAW2 || m_Osc1.m_uWaveform == SAW3)
		{
			double dRatioMod = m_dHSRatio*(m_ModulationMatrix.m_dDestinations[m_uSourceFoRatio]);
			m_Osc1.m_dOscFo = m_dOscPitch;
			m_Osc2.m_dOscFo = m_dOscPitch*dRatioMod;
		}

        m_Osc1.m_uModSourceAmp = m_dLFO1_DCAAmpModIntensity;
        m_Osc1.m_uModSourceFo = m_dLFO1_OscModIntensity;
		// update oscillators
		m_Osc1.updateOscillator();
		m_Osc2.updateOscillator();
		m_Osc3.updateOscillator();

		// --- DIGITAL AUDIO ENGINE BLOCK --- //
		double dOscMix = 0.0;
		if(m_uModMode == NONE)
			dOscMix = m_fOsc1Level*m_Osc1.doOscillate()*env1 +
                      m_fOsc2Level*m_Osc2.doOscillate()*env1 +
                      m_fOsc3Level*m_Osc3.doOscillate()*env1;
		else if(m_uModMode == RING)
			dOscMix = m_fOsc1Level*m_Osc1.doOscillate()*env1 +
                      m_fOsc2Level*m_Osc2.doOscillate()*env1 +
                      m_fOsc3Level*m_Osc3.doOscillate()*env1;

        dLeftOutput = dOscMix;
//		// apply the filter
//		dOscMix = m_LPF.doFilter(dOscMix);

		//Distortion
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

		// apply the DCA
		//m_DCA.doMonoDCA(dLPFOut, dLeftOutput, dRightOutput);

		return true;
	}
};
