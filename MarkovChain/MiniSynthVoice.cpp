#include "MiniSynthVoice.h"

CMiniSynthVoice::CMiniSynthVoice(void)
{
	// 1) --- declare your oscillators and filters
	// oscillators
	m_pOsc1 = &m_Osc1;
	m_pOsc2 = &m_Osc2;


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

}

void CMiniSynthVoice::update()
{
	CVoice::update();
}

