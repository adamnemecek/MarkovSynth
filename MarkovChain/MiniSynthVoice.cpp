#include "MiniSynthVoice.h"

CMiniSynthVoice::CMiniSynthVoice(void)
{
	// 1) --- declare your oscillators and filters
	// oscillators
	m_pOsc1 = &m_Osc1;
	m_pOsc2 = &m_Osc2;
//	m_pOsc3 = &m_Osc3;
//	m_pOsc4 = &m_Osc4;

	// filters
	m_pFilter1 = &m_LPF;
	m_pFilter2 = NULL;

	m_LPF.m_uNLP = ON;

	// 2) --- set any component specific stuff
	m_EG1.setEGMode(analog);

	// 1 is master of 2 in HSSAW mode
	m_Osc1.m_pBuddyOscillator = m_pOsc2;

	// default
	m_dHSRatio = 1.0;

	// these default to 1.0 in case user doesnt have a GUI control for them
	m_dLFO1_OscModIntensity = 1.0;
	m_dLFO1_Filter1ModIntensity = 1.0;
	m_dLFO1_Filter2ModIntensity = 1.0;
	
	m_dEG1_OscModIntensity = 1.0;
	m_dEG1_Filter1ModIntensity = 1.0;
	m_dEG1_Filter2ModIntensity = 1.0;
	m_dEG1_DCAAmpModIntensity = 1.0;

	m_dLFO1_DCAPanModIntensity = 1.0;

	m_dLFO1_DCAAmpModIntensity = 1.0;

	// for hard-sync
	m_uSourceFoRatio = DEST_OSC2_FO_RATIO;
	
	// --- FINISH setup of mod matrix
	//     NOTE: see CVoice::CVoice and CVoice::prepareForPlay()
	//           for basic setup of oscillators, filters, LFOs, EGs
	//           which is always the same for all synths
	//           Here, we add the things that are specific to this voice
	//
	// --- DCA Setup: set the source EG here
	m_DCA.m_uModSourceEG = DEST_DCA_EG;
	
	// --- The Mod matrix "wiring"
	// create a row for each source/destination pair
//	modMatrixRow* pRow;
//
//	// LFO1 -> ALL OSC1 FC
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_LFO1;
//	pRow->uDestinationIndex = DEST_ALL_OSC_FO;
//	pRow->pModIntensity = &m_dLFO1_OscModIntensity;
//	pRow->pModRange = &m_dOscFoModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//	
//	// EG1 -> ALL OSC1 FC
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_BIASED_EG1;
//	pRow->uDestinationIndex = DEST_ALL_OSC_FO;
//	pRow->pModIntensity = &m_dEG1_OscModIntensity;
//	pRow->pModRange = &m_dOscFoModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// LFO1 -> FILTER1 FC
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_LFO1;
//	pRow->uDestinationIndex = DEST_ALL_FILTER_FC;
//	pRow->pModIntensity = &m_dLFO1_Filter1ModIntensity;
//	pRow->pModRange = &m_dFilterModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// LFO1 -> PULSE WIDTH
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_LFO1;
//	pRow->uDestinationIndex = DEST_ALL_OSC_PULSEWIDTH;
//	pRow->pModIntensity = &m_dDefaultModIntensity;
//	pRow->pModRange = &m_dDefaultModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// LFO1 -> HARD SYNC RATIO
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_LFO1;
//	pRow->uDestinationIndex = DEST_ALL_OSC_FO_RATIO;
//	pRow->pModIntensity = &m_dDefaultModIntensity;
//	pRow->pModRange = &m_dOscHardSyncModRange;
//	pRow->uSourceTransform = TRANSFORM_BIPOLAR_TO_UNIPOLAR; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// EG1 -> FILTER1 FC
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_BIASED_EG1;
//	pRow->uDestinationIndex = DEST_ALL_FILTER_FC;
//	pRow->pModIntensity = &m_dEG1_Filter1ModIntensity;
//	pRow->pModRange = &m_dFilterModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// EG1 -> DCA EG
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_EG1;
//	pRow->uDestinationIndex = DEST_DCA_EG;
//	pRow->pModIntensity = &m_dEG1_DCAAmpModIntensity; 	
//	pRow->pModRange = &m_dDefaultModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; 
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// LFO1 (-1 -> +1) -> DCA Amp Mod (0->1)
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_LFO1;
//	pRow->uDestinationIndex = DEST_DCA_AMP;
//	pRow->pModIntensity = &m_dLFO1_DCAAmpModIntensity;
//	pRow->pModRange = &m_dAmpModRange;
//	pRow->uSourceTransform = TRANSFORM_BIPOLAR_TO_UNIPOLAR; // convert LFO to unipolar
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
//
//	// LFO1 (-1 -> +1) -> DCA Pan Mod (-1->1)
//	pRow = new modMatrixRow;
//	pRow->uSourceIndex = SOURCE_LFO1;
//	pRow->uDestinationIndex = DEST_DCA_PAN;
//	pRow->pModIntensity = &m_dLFO1_DCAPanModIntensity;
//	pRow->pModRange = &m_dDefaultModRange;
//	pRow->uSourceTransform = TRANSFORM_NONE; // none
//
//	m_ModulationMatrix.addModMatrixRow(pRow);
}

CMiniSynthVoice::~CMiniSynthVoice(void)
{
}

void CMiniSynthVoice::setSampleRate(double dSampleRate)
{
	CVoice::setSampleRate(dSampleRate);
}

void CMiniSynthVoice::prepareForPlay()
{
	CVoice::prepareForPlay();

	reset();
}

void CMiniSynthVoice::reset()
{
	CVoice::reset();

	m_dPortamentoInc = 0.0;

	m_Osc1.m_uWaveform = SAW1;
	m_Osc2.m_uWaveform = SAW1;
//	m_Osc3.m_uWaveform = SAW1;
//	m_Osc4.m_uWaveform = NOISE;
}

void CMiniSynthVoice::update()
{
	CVoice::update();
}

