#pragma once
#include "synthfunctions.h"

#include "DCA.h"
#include "Oscillator.h"
#include "LFO.h"
#include "EnvelopeGenerator.h"
#include "Filter.h"
#include "ModulationMatrix.h"

class CVoice
{
public:
	CVoice(void);
	virtual ~CVoice(void);

	// NOTE: public; this is shared by sources and destinations
	CModulationMatrix m_ModulationMatrix;

	UINT m_uSourceFoRatio; // for hard sync

protected:
	double m_dSampleRate;

	// this will be different for the voices
	UINT m_uVoiceMode;

	// legato on/off
	UINT m_uLegatoMode;
	enum {mono,legato};

	// current pitch
	double m_dOscPitch;

	// pitch pending from a note-steal operation
	double m_dOscPitchPending;

	// for portamento: Starting Pitch
	double m_dOscPitchPortamentoStart;

	// a counter/inc pair for the timebase
	double m_dModuloPortamento;
	double m_dPortamentoInc;

	// the amount of portamento in semitones
	double m_dPortamentoSemitones;

	// flag that a new note is pending (steal mode)
	bool m_bNotePending;
	
	// enum for EG state variable so we can query the EG	
	enum {off, attack, decay, sustain, release, shutdown};
	
	// for LFOs
	enum {sine,usaw,dsaw,tri,square,expo,rsh,qrsh};

	// for PITCHED Oscillators
	enum {SINE,SAW1,SAW2,SAW3,TRI,SQUARE,NOISE,PNOISE};
	
	// for NLP and other ON/OFF switches
	enum{OFF,ON}; 

	// for EG mode
	enum {analog, digital};
	
	// 4 oscillators
	COscillator* m_pOsc1;
	COscillator* m_pOsc2;
	COscillator* m_pOsc3;
	COscillator* m_pOsc4;

	// 2 filters; 2 mono or 1 stereo
	CFilter* m_pFilter1;
	CFilter* m_pFilter2;

	// 4 EGs
	CEnvelopeGenerator m_EG1;
	CEnvelopeGenerator m_EG2;
	CEnvelopeGenerator m_EG3;
	CEnvelopeGenerator m_EG4;
	
	// 2 LFOs	
	CLFO m_LFO1;
	CLFO m_LFO2;

	// 1 DCA (can be mono or stereo)
	CDCA m_DCA;

	// --- default mod matrix ranges and intensities
	double m_dDefaultModIntensity;
	double m_dDefaultModRange;
	double m_dFilterModRange;
	double m_dAmpModRange;
	double m_dOscFoModRange;
	double m_dOscFoPitchBendModRange;
	double m_dOscHardSyncModRange;
	double m_dFilterKeyTrackIntensity;

public:
	// --- our state; may be different from Oscillator states
	bool m_bNoteOn;

	// --- MIDI stuff
	UINT m_uMIDINoteNumber;
	UINT m_uMIDINoteNumberPending;
	
	UINT m_uMIDIVelocity;
	UINT m_uMIDIVelocityPending;

	// --- Call Forwarding Messages to OSCILLATORS
	inline void setOctave(int n)
	{
		if(m_pOsc1)m_pOsc1->m_nOctave = n;
		if(m_pOsc2)m_pOsc2->m_nOctave = n;
		if(m_pOsc3)m_pOsc3->m_nOctave = n;
		if(m_pOsc4)m_pOsc4->m_nOctave = n;
	}

	inline void setSemitones(int n)
	{
		if(m_pOsc1)m_pOsc1->m_nSemitones = n;
		if(m_pOsc2)m_pOsc2->m_nSemitones = n;
		if(m_pOsc3)m_pOsc3->m_nSemitones = n;
		if(m_pOsc4)m_pOsc4->m_nSemitones = n;
	}

	inline void setCents(int n)
	{
		if(m_pOsc1)m_pOsc1->m_nCents = n;
		if(m_pOsc2)m_pOsc2->m_nCents = n;
		if(m_pOsc3)m_pOsc3->m_nCents = n;
		if(m_pOsc4)m_pOsc4->m_nCents = n;
	}

	inline void setPulseWidthControl(double d)
	{
		if(m_pOsc1)m_pOsc1->m_dPulseWidthControl = d;
		if(m_pOsc2)m_pOsc2->m_dPulseWidthControl = d;
		if(m_pOsc3)m_pOsc3->m_dPulseWidthControl = d;
		if(m_pOsc4)m_pOsc4->m_dPulseWidthControl = d;
	}

	// osc mix values based on dB (GUI) controls
	inline void setOscAmplitude(UINT uOsc, double dLevel)
	{
		if(uOsc == 0)
			if(m_pOsc1)m_pOsc1->m_dAmplitude = dLevel;
		if(uOsc == 1)
			if(m_pOsc2)m_pOsc2->m_dAmplitude = dLevel;
		if(uOsc == 2)
			if(m_pOsc3)m_pOsc3->m_dAmplitude = dLevel;
		if(uOsc == 3)
			if(m_pOsc4)m_pOsc4->m_dAmplitude = dLevel;
	}
	
	// osc mix values based on dB (GUI) controls
	inline void setOscAmplitude_dB(UINT uOsc, double dLevel_dB)
	{
		double dLevel = dLevel_dB == -96.0 ? 0.0 : pow(10.0, dLevel_dB/20.0);
		setOscAmplitude(uOsc, dLevel);
	}

	// DX uses 0->99 in a dB fashion which corresponds to DX controls
	inline void setDXOpAmplitude(UINT uOperator, double dDXLevel)
	{
		// if level = 0, leave it otherwise convert
		if(dDXLevel != 0.0)
		{
			// empirically found using a DX100
			dDXLevel -= 99.0;
			dDXLevel /= 1.32;

			// set dB-ish version
			dDXLevel = (pow(10.0, dDXLevel/20.0));
		}
		
		// call normal method
		setOscAmplitude(uOperator, dDXLevel);
	}
	
	// set waveform directly
	inline void setOscWaveform(UINT uOsc, UINT uWaveform)
	{
		switch(uOsc)
		{
			case 0:
				if(m_pOsc1)m_pOsc1->m_uWaveform = uWaveform;
				break;
			case 1:
				if(m_pOsc2)m_pOsc2->m_uWaveform = uWaveform;
				break;
			case 2:
				if(m_pOsc3)m_pOsc3->m_uWaveform = uWaveform;
				break;
			case 3:
				if(m_pOsc4)m_pOsc4->m_uWaveform = uWaveform;
				break;
			default: 
				break;
		}
	}
	// for FM or PM synthesis or other interesting deviations
	inline void setOscFoRatio(UINT uOsc, double dRatio)
	{
		switch(uOsc)
		{
			case 0:
				if(m_pOsc1)m_pOsc1->m_dFoRatio = dRatio;
				break;
			case 1:
				if(m_pOsc2)m_pOsc2->m_dFoRatio = dRatio;
				break;
			case 2:
				if(m_pOsc3)m_pOsc3->m_dFoRatio = dRatio;
				break;
			case 3:
				if(m_pOsc4)m_pOsc4->m_dFoRatio = dRatio;
				break;
			default: 
				break;
		}
	}
	
	// for detuning oscillators in the voice
	inline void setDetuneCents(UINT uOsc, double dDetuneCents)
	{
		switch(uOsc)
		{
			case 0:
				if(m_pOsc1)m_pOsc1->m_nCents = dDetuneCents;
				break;
			case 1:
				if(m_pOsc2)m_pOsc2->m_nCents = dDetuneCents;
				break;
			case 2:
				if(m_pOsc3)m_pOsc3->m_nCents = dDetuneCents;
				break;
			case 3:
				if(m_pOsc4)m_pOsc4->m_nCents = dDetuneCents;
				break;
			default: 
				break;
		}
	}

	// --- Modulation Matrix Sources
	//
	// --- MIDI Controller and Mods -- turn on smoothing when user first moves control
	inline void setPitchBendMod(double d)
	{
		m_ModulationMatrix.m_dSources[SOURCE_PITCHBEND] = d;
		m_ModulationMatrix.setControlSmoother(SOURCE_PITCHBEND);
	}
	inline void setModWheelMod(double d)
	{
		m_ModulationMatrix.m_dSources[SOURCE_MODWHEEL] = d;
		m_ModulationMatrix.setControlSmoother(SOURCE_MODWHEEL);
	}
	inline void setMIDIVolumeCC07(double d)
	{
		m_ModulationMatrix.m_dSources[SOURCE_MIDI_VOLUME_CC07] = d;
		m_ModulationMatrix.setControlSmoother(SOURCE_MIDI_VOLUME_CC07);
	}
	inline void setMIDIPanCC10(double d)
	{
		m_ModulationMatrix.m_dSources[SOURCE_MIDI_PAN_CC10] = d;
		m_ModulationMatrix.setControlSmoother(SOURCE_MIDI_PAN_CC10);
	}
	inline void setMIDIExpressionCC11(double d)
	{
		m_ModulationMatrix.m_dSources[SOURCE_MIDI_EXPRESSION_CC11] = d;
		m_ModulationMatrix.setControlSmoother(SOURCE_MIDI_EXPRESSION_CC11);
	}

	// --- filter keytrack is done at Voice level
	inline void setFilterKeyTrackIntensity(double d){m_dFilterKeyTrackIntensity = d;}
	
	// pbend range; default is +/- 2 semitones
	inline void setPitchBendModRange(double d){m_dOscFoPitchBendModRange = d;}

	// --- Call Forwarding Messages to FILTERS
	inline void setFilter1Cutoff(double dFc){m_pFilter1->m_dFcControl = dFc;}
	inline void setFilter2Cutoff(double dFc){m_pFilter2->m_dFcControl = dFc;}

	inline void setFilter1Q(double dQ){m_pFilter1->setQControl(dQ);}
	inline void setFilter2Q(double dQ){m_pFilter2->setQControl(dQ);}

	// --- Call Forwarding Messages to LFOs
	inline void setLFO1Waveform(UINT u){m_LFO1.m_uWaveform = u;}
	inline void setLFO2Waveform(UINT u){m_LFO2.m_uWaveform = u;}

	inline void setLFO1Rate(double d){m_LFO1.m_dOscFo = d;}
	inline void setLFO2Rate(double d){m_LFO2.m_dOscFo = d;}

	inline void setLFO1Amplitude(double d){m_LFO1.m_dAmplitude = d;}
	inline void setLFO2Amplitude(double d){m_LFO2.m_dAmplitude = d;}

	// --- Call Forwarding Messages to EG(s)
	inline void setEG1LegatoMode(bool b){m_EG1.m_bLegatoMode = b;}
	inline void setEG2LegatoMode(bool b){m_EG2.m_bLegatoMode = b;}
	inline void setEG3LegatoMode(bool b){m_EG3.m_bLegatoMode = b;}
	inline void setEG4LegatoMode(bool b){m_EG4.m_bLegatoMode = b;}

	inline void setEG1ResetToZero(bool b){m_EG1.m_bResetToZero = b;}
	inline void setEG2ResetToZero(bool b){m_EG2.m_bResetToZero = b;}
	inline void setEG3ResetToZero(bool b){m_EG3.m_bResetToZero = b;}
	inline void setEG4ResetToZero(bool b){m_EG4.m_bResetToZero = b;}

	inline void setEG1AttackTime_mSec(double d){m_EG1.setAttackTime_mSec(d);}
	inline void setEG2AttackTime_mSec(double d){m_EG2.setAttackTime_mSec(d);}
	inline void setEG3AttackTime_mSec(double d){m_EG3.setAttackTime_mSec(d);}
	inline void setEG4AttackTime_mSec(double d){m_EG4.setAttackTime_mSec(d);}

	inline void setEG1DecayTime_mSec(double d){m_EG1.setDecayTime_mSec(d);}
	inline void setEG2DecayTime_mSec(double d){m_EG2.setDecayTime_mSec(d);}
	inline void setEG3DecayTime_mSec(double d){m_EG3.setDecayTime_mSec(d);}
	inline void setEG4DecayTime_mSec(double d){m_EG4.setDecayTime_mSec(d);}

	inline void setEG1ReleaseTime_mSec(double d){m_EG1.setReleaseTime_mSec(d);}
	inline void setEG2ReleaseTime_mSec(double d){m_EG2.setReleaseTime_mSec(d);}
	inline void setEG3ReleaseTime_mSec(double d){m_EG3.setReleaseTime_mSec(d);}
	inline void setEG4ReleaseTime_mSec(double d){m_EG4.setReleaseTime_mSec(d);}

	inline void setEG1SustainLevel(double d){m_EG1.setSustainLevel(d);}
	inline void setEG2SustainLevel(double d){m_EG2.setSustainLevel(d);}
	inline void setEG3SustainLevel(double d){m_EG3.setSustainLevel(d);}
	inline void setEG4SustainLevel(double d){m_EG4.setSustainLevel(d);}	

	// --- Call Forwarding Messages to DCA
	inline void setDCAAmplitude_dB(double d){m_DCA.setAmplitude_dB(d);}
	
	// -- set portamento timer
	inline void setPortamentoTime_mSec(double dTime)
	{
		if(dTime == 0.0)
			m_dPortamentoInc = 0.0;
		else
			m_dPortamentoInc = 1000.0/dTime/m_dSampleRate;
	}



	// WILL need to override this for FM Instrument or any parallel-osc algorithm with multiple output amp EGs
	inline virtual bool isActiveVoice()
	{	
		if(m_bNoteOn && m_EG1.isActive()) 
			return true;

		return false;
	}
	
	// we are in a state to accept a noteOff message
	// WILL need to override this for FM Instrument or any parallel-osc algorithm with multiple output amp EGs
	inline virtual bool canNoteOff()
	{	
		if(m_bNoteOn && m_EG1.canNoteOff())
			return true;

		return false;
	}
	
	// WILL need to override this for FM Instrument or any parallel-osc algorithm with multiple output amp EGs
	// this will be called to turn off m_bNoteOn so don't use it in this logic
	inline virtual bool isVoiceDone()
	{	
		if(m_EG1.getState() == off)
			return true;

		return false;
	}
	
	// MAY need to override this for FM Instrument or any parallel-osc algorithm with multiple output amp EGs
	inline virtual bool inLegatoMode()
	{	
		return m_EG1.m_bLegatoMode;
	}

	// set sustain override on each EG; common to all synths
	inline virtual void setSustainOverride(bool b)
	{
		if(isActiveVoice())
		{
			m_EG1.setSustainOverride(b);
			m_EG2.setSustainOverride(b);
			m_EG3.setSustainOverride(b);
			m_EG4.setSustainOverride(b);
		}
		else
		{
			m_EG1.setSustainOverride(false);
			m_EG2.setSustainOverride(false);
			m_EG3.setSustainOverride(false);
			m_EG4.setSustainOverride(false);
		}
	}
	
	// --- override for your synth
	inline virtual void setVoiceMode(UINT u)
	{
		m_uVoiceMode = u;
	}

	// --- overrides
	virtual void setSampleRate(double dSampleRate);
	virtual void prepareForPlay();
	virtual void update();
	virtual void reset();
	
	// --- MIDI methods
	inline void noteOn(UINT uMIDINote, UINT uMIDIVelocity, double dFrequency, double dLastNoteFrequency)
	{
		// save
		m_dOscPitch = dFrequency;

		// is our voice avaialble?
		if(!m_bNoteOn && !m_bNotePending)
		{
			// save the note number (for voice steal later)
			m_uMIDINoteNumber = uMIDINote;
			
			// the DCA needs this
			m_uMIDIVelocity = uMIDIVelocity;
			
			// set the velocity info 
			m_ModulationMatrix.m_dSources[SOURCE_VELOCITY] = (double)m_uMIDIVelocity;

			// set note number
			m_ModulationMatrix.m_dSources[SOURCE_MIDI_NOTE_NUM] = (double)uMIDINote;

			// clear smoothers
			// m_ModulationMatrix.clearControlSmoothers();

			// set the OSC
			if(m_dPortamentoInc > 0.0 && dLastNoteFrequency > 0)
			{	
				m_dModuloPortamento = 0.0;
				m_dPortamentoSemitones = semitonesBetweenFrequencies(dLastNoteFrequency, dFrequency);
				m_dOscPitchPortamentoStart = dLastNoteFrequency;
				
				if(m_pOsc1)m_pOsc1->m_dOscFo = m_dOscPitchPortamentoStart;
				if(m_pOsc2)m_pOsc2->m_dOscFo = m_dOscPitchPortamentoStart;
				if(m_pOsc3)m_pOsc3->m_dOscFo = m_dOscPitchPortamentoStart;
				if(m_pOsc4)m_pOsc4->m_dOscFo = m_dOscPitchPortamentoStart;
			}
			else
			{
				if(m_pOsc1)m_pOsc1->m_dOscFo = m_dOscPitch;
				if(m_pOsc2)m_pOsc2->m_dOscFo = m_dOscPitch;
				if(m_pOsc3)m_pOsc3->m_dOscFo = m_dOscPitch;
				if(m_pOsc4)m_pOsc4->m_dOscFo = m_dOscPitch;
			}

			// set MIDI note number (needed for Sample based osc)
			if(m_pOsc1)m_pOsc1->m_uMIDINoteNumber = m_uMIDINoteNumber;
			if(m_pOsc2)m_pOsc2->m_uMIDINoteNumber = m_uMIDINoteNumber;
			if(m_pOsc3)m_pOsc3->m_uMIDINoteNumber = m_uMIDINoteNumber;
			if(m_pOsc4)m_pOsc4->m_uMIDINoteNumber = m_uMIDINoteNumber;

			// start; NOTE this will reset and update()
			if(m_pOsc1)m_pOsc1->startOscillator();
			if(m_pOsc2)m_pOsc2->startOscillator();
			if(m_pOsc3)m_pOsc3->startOscillator();
			if(m_pOsc4)m_pOsc4->startOscillator();

			// start EGs
			m_EG1.startEG();
			m_EG2.startEG();
			m_EG3.startEG();
			m_EG4.startEG();

			// start LFOs
			m_LFO1.startOscillator();
			m_LFO2.startOscillator();

			// we are rendering!
			m_bNoteOn = true;

			// bail
			return;
		}

		// IF we get here, we are playing a note and need to steal it
		// Save PENDING note number and velocity
		if(m_bNotePending && m_uMIDINoteNumberPending == uMIDINote)
			return;

		m_uMIDINoteNumberPending = uMIDINote;
			
		m_dOscPitchPending = dFrequency;
		m_uMIDIVelocityPending = uMIDIVelocity;
	
		// set the flag that we have a note pending
		m_bNotePending = true;

		// set portamento stuff
		if(m_dPortamentoInc > 0.0 && dLastNoteFrequency > 0)
		{
			if(m_dModuloPortamento > 0.0)
			{
				double dPortamentoPitchMult = pitchShiftMultiplier(m_dModuloPortamento*m_dPortamentoSemitones);
				m_dOscPitchPortamentoStart = m_dOscPitchPortamentoStart*dPortamentoPitchMult;
			}
			else
			{	
				m_dOscPitchPortamentoStart = dLastNoteFrequency;
			}

			m_dModuloPortamento = 0.0;
			m_dPortamentoSemitones = semitonesBetweenFrequencies(m_dOscPitchPortamentoStart, dFrequency);
		}

		// and shutdown the EGs
		m_EG1.shutDown();
		m_EG2.shutDown();
		m_EG3.shutDown();
		m_EG4.shutDown();
	}


	inline virtual void noteOff(UINT uMIDINoteNumber)
	{
		if(m_bNoteOn)
		{
			if(m_bNotePending && (uMIDINoteNumber == m_uMIDINoteNumberPending))
			{
				m_bNotePending = false;
				return;
			}
		
			if(uMIDINoteNumber != m_uMIDINoteNumber)
				return;

			// are we already in Release or Shutdown or Off?
//			if(m_EG1.getState() == release || m_EG1.getState() == shutdown || m_EG1.getState() == off)
			if(m_EG1.getState() == release || m_EG1.getState() == off)
				return;
			
			m_EG1.noteOff();
			m_EG2.noteOff();
			m_EG3.noteOff();
			m_EG4.noteOff();
		}
	}
	// returns true if voice is still ON
	inline virtual bool doVoice(double& dLeftOutput, double& dRightOutput)
	{
		// clear destinations
		dLeftOutput = 0.0;
		dRightOutput = 0.0;

		// bail if no note 
		if(!m_bNoteOn)
			return false;

		// did EG finish? - its the flag for us as a voice
		if(isVoiceDone() || m_bNotePending)
		{
			// did EG finish with NO note pending?
			if(isVoiceDone() && !m_bNotePending)
			{
				// shut off and reset everything
				if(m_pOsc1)m_pOsc1->stopOscillator();
				if(m_pOsc2)m_pOsc2->stopOscillator();
				if(m_pOsc3)m_pOsc3->stopOscillator();
				if(m_pOsc4)m_pOsc4->stopOscillator();

				// need this in case of steal mode
				if(m_pOsc1)m_pOsc1->reset();
				if(m_pOsc2)m_pOsc2->reset();
				if(m_pOsc3)m_pOsc3->reset();
				if(m_pOsc4)m_pOsc4->reset();

				// stop the LFOs
				m_LFO1.stopOscillator();
				m_LFO2.stopOscillator();

				// stop the EGs
				m_EG1.reset();
				m_EG2.reset();
				m_EG3.reset();
				m_EG4.reset();

				// not more note
				m_bNoteOn = false;

				// done
				return false;
			}
			else if(m_bNotePending && (isVoiceDone() || inLegatoMode()))// note pending so turn it on
			{
				// transfer informationn from PENDING values
				m_uMIDINoteNumber = m_uMIDINoteNumberPending;
				m_uMIDIVelocity = m_uMIDIVelocityPending;							
				m_dOscPitch = m_dOscPitchPending;

				// set note number
				m_ModulationMatrix.m_dSources[SOURCE_MIDI_NOTE_NUM] = (double)m_uMIDINoteNumber;

				if(!inLegatoMode())
					// new velocity value
					m_ModulationMatrix.m_dSources[SOURCE_VELOCITY] = (double)m_uMIDIVelocity;
				
				// portamento on? use start pitch
				double dPitch = m_dPortamentoInc > 0.0 ? m_dOscPitchPortamentoStart : m_dOscPitch;

				if(m_pOsc1)m_pOsc1->m_dOscFo = dPitch;
				if(m_pOsc2)m_pOsc2->m_dOscFo = dPitch;
				if(m_pOsc3)m_pOsc3->m_dOscFo = dPitch;
				if(m_pOsc4)m_pOsc4->m_dOscFo = dPitch;
		
				// MIDI note number
				if(m_pOsc1)m_pOsc1->m_uMIDINoteNumber = m_uMIDINoteNumber;
				if(m_pOsc2)m_pOsc2->m_uMIDINoteNumber = m_uMIDINoteNumber;
				if(m_pOsc3)m_pOsc3->m_uMIDINoteNumber = m_uMIDINoteNumber;
				if(m_pOsc4)m_pOsc4->m_uMIDINoteNumber = m_uMIDINoteNumber;

				// update so new pitch is used
				if(m_pOsc1)m_pOsc1->updateOscillator();
				if(m_pOsc2)m_pOsc2->updateOscillator();
				if(m_pOsc3)m_pOsc3->updateOscillator();
				if(m_pOsc4)m_pOsc4->updateOscillator();


				// crank the EGs back up
				m_EG1.startEG();
				m_EG2.startEG();
				m_EG3.startEG();
				m_EG4.startEG();

				// start LFOs
				m_LFO1.startOscillator();
				m_LFO2.startOscillator();

				// clear flag
				m_bNotePending = false;
			}
		}
		
		// --- PORTAMENTO BLOCK --- //
		if(m_dPortamentoInc > 0.0 && m_pOsc1->m_dOscFo != m_dOscPitch)
		{
			if(m_dModuloPortamento >= 1.0)
			{
				m_dModuloPortamento = 0.0;

				// target pitch has now been hit
				if(m_pOsc1) m_pOsc1->m_dOscFo = m_dOscPitch;
				if(m_pOsc2) m_pOsc2->m_dOscFo = m_dOscPitch;
				if(m_pOsc3) m_pOsc3->m_dOscFo = m_dOscPitch;
				if(m_pOsc4) m_pOsc4->m_dOscFo = m_dOscPitch;
			}
			else
			{
				double dPortamentoPitch = m_dOscPitchPortamentoStart*pitchShiftMultiplier(m_dModuloPortamento*m_dPortamentoSemitones);
			
				if(m_pOsc1) m_pOsc1->m_dOscFo = dPortamentoPitch;
				if(m_pOsc2) m_pOsc2->m_dOscFo = dPortamentoPitch;
				if(m_pOsc3) m_pOsc3->m_dOscFo = dPortamentoPitch;
				if(m_pOsc4) m_pOsc4->m_dOscFo = dPortamentoPitch;
	
				m_dModuloPortamento += m_dPortamentoInc;
			}
		}

		// if we make it here, we are rendering
		// NOTE: Derived class method must call updateOscillator() 
		//       for portamento to work
		return true;
	}
};
