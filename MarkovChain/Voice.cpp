#include "Voice.h"

CVoice::CVoice(void)
{
	m_bNoteOn = false;
	m_bNotePending = false;
	m_dSampleRate = 44100;
	m_uVoiceMode = 0;  // this will vary in meaning depending on synth

	m_dOscPitch = 440.0;
	m_dOscPitchPending = 440.0;
	m_dOscPitchPortamentoStart = 440.0;
	
	m_dModuloPortamento = 0.0;
	m_dPortamentoInc = 0.0;
	m_dPortamentoSemitones = 0.0;

	m_uLegatoMode = mono;

	m_pOsc1 = NULL;
	m_pOsc2 = NULL;
	m_pOsc3 = NULL;
	m_pOsc4 = NULL;

	m_pFilter1 = NULL;
	m_pFilter2 = NULL;

	m_dDefaultModIntensity = 1.0;
	m_dDefaultModRange = 1.0;

	m_dFilterKeyTrackIntensity = 1.0; // off

	m_dFilterModRange = FILTER_FC_MOD_RANGE;
	m_dOscFoModRange = OSC_FO_MOD_RANGE;
	m_dOscFoPitchBendModRange = OSC_PITCHBEND_MOD_RANGE;
	m_dAmpModRange = AMP_MOD_RANGE;
	m_dOscHardSyncModRange = OSC_HARD_SYNC_RATIO_RANGE;

	// for any voice-specific modulation
	m_uSourceFoRatio = DEST_NONE;

	// --- setup the mod matrix for defaults
	//     LFOs, EGs only
	//     Filters and Oscillators done in 
	//     derived class constructor
	m_ModulationMatrix.clearSources();

	m_LFO1.m_pModulationMatrix = &m_ModulationMatrix;
	m_LFO1.m_uModDestOutput1 = SOURCE_LFO1;
	m_LFO1.m_uModDestOutput2 = SOURCE_LFO1Q;
	// Default Connection: Mod Wheel -> LFO1 Depth
	// --- LFO1 Source Amplitude
	m_LFO1.m_uModSourceAmp = DEST_LFO1_OUTPUT_AMP;

	m_LFO2.m_pModulationMatrix = &m_ModulationMatrix;
	m_LFO2.m_uModDestOutput1 = SOURCE_LFO2;
	m_LFO2.m_uModDestOutput2 = SOURCE_LFO2Q;

	m_EG1.m_pModulationMatrix = &m_ModulationMatrix;
	m_EG1.m_uModDestEGOutput = SOURCE_EG1;
	m_EG1.m_uModDestBiasedEGOutput = SOURCE_BIASED_EG1;

	m_EG2.m_pModulationMatrix = &m_ModulationMatrix;
	m_EG2.m_uModDestEGOutput = SOURCE_EG2;
	m_EG2.m_uModDestBiasedEGOutput = SOURCE_BIASED_EG2;

	m_EG3.m_pModulationMatrix = &m_ModulationMatrix;
	m_EG3.m_uModDestEGOutput = SOURCE_EG3;
	m_EG3.m_uModDestBiasedEGOutput = SOURCE_BIASED_EG3;

	m_EG4.m_pModulationMatrix = &m_ModulationMatrix;
	m_EG4.m_uModDestEGOutput = SOURCE_EG4;
	m_EG4.m_uModDestBiasedEGOutput = SOURCE_BIASED_EG4;

	// --- DCA Setup: 
	m_DCA.m_pModulationMatrix = &m_ModulationMatrix;
	m_DCA.m_uModSourceEG = DEST_NONE; // this must be connected on derived class, if used normally EG1
	m_DCA.m_uModSourceAmp = DEST_DCA_AMP;
	m_DCA.m_uModSourceVelocity = DEST_DCA_VELOCITY;
	m_DCA.m_uModSourcePan = DEST_DCA_PAN;

	// --- The Mod matrix "wiring" for DEFAULTS for all synths
	// create a row for each source/destination pair
	modMatrixRow* pRow;

	// VELOCITY -> DCA VEL
	pRow = new modMatrixRow;
	pRow->uSourceIndex = SOURCE_VELOCITY;
	pRow->uDestinationIndex = DEST_DCA_VELOCITY;
	pRow->pModIntensity = &m_dDefaultModIntensity; // 1.0 for this synth
	pRow->pModRange = &m_dDefaultModRange;
	pRow->uSourceTransform = TRANSFORM_NONE; 

	m_ModulationMatrix.addModMatrixRow(pRow);

	// PITCHBEND -> PITCHBEND
	pRow = new modMatrixRow;
	pRow->uSourceIndex = SOURCE_PITCHBEND;
	pRow->uDestinationIndex = DEST_ALL_OSC_FO;
	pRow->pModIntensity = &m_dDefaultModIntensity;
	pRow->pModRange = &m_dOscFoPitchBendModRange;
	pRow->uSourceTransform = TRANSFORM_NONE; 

	m_ModulationMatrix.addModMatrixRow(pRow);
	
	// MIDI Volume CC07
	pRow = new modMatrixRow;
	pRow->uSourceIndex = SOURCE_MIDI_VOLUME_CC07;
	pRow->uDestinationIndex = DEST_DCA_AMP;
	pRow->pModIntensity = &m_dDefaultModIntensity;
	pRow->pModRange = &m_dAmpModRange;
	pRow->uSourceTransform = TRANSFORM_INVERT_MIDI_NORMALIZE; 

	m_ModulationMatrix.addModMatrixRow(pRow);

	// MIDI Pan CC10
	pRow = new modMatrixRow;
	pRow->uSourceIndex = SOURCE_MIDI_PAN_CC10;
	pRow->uDestinationIndex = DEST_DCA_PAN;
	pRow->pModIntensity = &m_dDefaultModIntensity;
	pRow->pModRange = &m_dDefaultModRange;
	pRow->uSourceTransform = TRANSFORM_MIDI_TO_PAN; 

	m_ModulationMatrix.addModMatrixRow(pRow);

	// MIDI Mod Wheel (0->127) -> LFO1 Depth (0->1)
	pRow = new modMatrixRow;
	pRow->uSourceIndex = SOURCE_MODWHEEL;
	pRow->uDestinationIndex = DEST_LFO1_OUTPUT_AMP;
	pRow->pModIntensity = &m_dDefaultModIntensity;
	pRow->pModRange = &m_dDefaultModRange;
	pRow->uSourceTransform = TRANSFORM_MIDI_NORMALIZE; // 0->127 -->> 0->1

	m_ModulationMatrix.addModMatrixRow(pRow);
}

CVoice::~CVoice(void)
{
}

void CVoice::setSampleRate(double dSampleRate)
{
	m_dSampleRate = dSampleRate;

	if(m_pOsc1)m_pOsc1->setSampleRate(dSampleRate);
	if(m_pOsc2)m_pOsc2->setSampleRate(dSampleRate);
	if(m_pOsc3)m_pOsc3->setSampleRate(dSampleRate);
	if(m_pOsc4)m_pOsc4->setSampleRate(dSampleRate);

	if(m_pFilter1)m_pFilter1->setSampleRate(dSampleRate);
	if(m_pFilter2)m_pFilter2->setSampleRate(dSampleRate);

	m_EG1.setSampleRate(dSampleRate);
	m_EG2.setSampleRate(dSampleRate);
	m_EG3.setSampleRate(dSampleRate);
	m_EG4.setSampleRate(dSampleRate);

	m_LFO1.setSampleRate(dSampleRate);
	m_LFO2.setSampleRate(dSampleRate);
}

// power on defaults, one time init
void CVoice::prepareForPlay()
{
	// power on defaults
	setMIDIVolumeCC07(127);
	setMIDIPanCC10(64);

	m_EG1.prepareForPlay();
	m_EG2.prepareForPlay();
	m_EG3.prepareForPlay();
	m_EG4.prepareForPlay();

	// setup the mod matrix pointers since oscillators are defined by now
	if(m_pOsc1)
	{
		m_pOsc1->m_pModulationMatrix = &m_ModulationMatrix;
		m_pOsc1->m_uModSourceFo = DEST_OSC1_FO;
		m_pOsc1->m_uModSourcePulseWidth = DEST_OSC1_PULSEWIDTH;
		m_pOsc1->m_uModSourceAmp = DEST_NONE; // NOTE: amp mods 1->0 so do NOT set unless you have a real source
	}
	if(m_pOsc2)
	{
		m_pOsc2->m_pModulationMatrix = &m_ModulationMatrix;
		m_pOsc2->m_uModSourceFo = DEST_OSC2_FO;
		m_pOsc2->m_uModSourcePulseWidth = DEST_OSC2_PULSEWIDTH;
		m_pOsc2->m_uModSourceAmp = DEST_NONE; // NOTE: amp mods 1->0 so do NOT set unless you have a real source
	}
	if(m_pOsc3)
	{
		m_pOsc3->m_pModulationMatrix = &m_ModulationMatrix;
		m_pOsc3->m_uModSourceFo = DEST_OSC3_FO;
		m_pOsc3->m_uModSourcePulseWidth = DEST_OSC3_PULSEWIDTH;
		m_pOsc3->m_uModSourceAmp = DEST_NONE; // NOTE: amp mods 1->0 so do NOT set unless you have a real source
	}
	if(m_pOsc4)
	{
		m_pOsc4->m_pModulationMatrix = &m_ModulationMatrix;
		m_pOsc4->m_uModSourceFo = DEST_OSC4_FO;
		m_pOsc4->m_uModSourcePulseWidth = DEST_OSC4_PULSEWIDTH;
		m_pOsc4->m_uModSourceAmp = DEST_NONE; // NOTE: amp mods 1->0 so do NOT set unless you have a real source
	}

	// same for filters
	if(m_pFilter1)
	{
		m_pFilter1->m_pModulationMatrix = &m_ModulationMatrix;
		m_pFilter1->m_uModSourceFc = DEST_FILTER1_FC;
	}

	if(m_pFilter2)
	{
		m_pFilter2->m_pModulationMatrix = &m_ModulationMatrix;
		m_pFilter2->m_uModSourceFc = DEST_FILTER1_FC;
	}

}

void CVoice::update()
{
	if(m_pOsc1)m_pOsc1->updateOscillator();
	if(m_pOsc2)m_pOsc2->updateOscillator();
	if(m_pOsc3)m_pOsc3->updateOscillator();
	if(m_pOsc4)m_pOsc4->updateOscillator();

	if(m_pFilter1)m_pFilter1->updateFilter();
	if(m_pFilter2)m_pFilter2->updateFilter();

	m_LFO1.updateOscillator();
	m_LFO2.updateOscillator();

	m_DCA.updateDCA();
}
	
void CVoice::reset()
{
	if(m_pOsc1)m_pOsc1->reset();
	if(m_pOsc2)m_pOsc2->reset();
	if(m_pOsc3)m_pOsc3->reset();
	if(m_pOsc4)m_pOsc4->reset();
	
	if(m_pFilter1)m_pFilter1->reset();
	if(m_pFilter2)m_pFilter2->reset();

	m_EG1.reset();
	m_EG2.reset();
	m_EG3.reset();
	m_EG4.reset();

	m_LFO1.reset();
	m_LFO2.reset();

	m_DCA.reset();
}
