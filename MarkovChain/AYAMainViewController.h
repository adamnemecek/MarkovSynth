//
//  AYAMainViewController.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/10/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AYAConnection.h"
#import "AYAConnectionManager.h"
#import "AYANodeView.h"
#import "AYATableViewController.h"
#import "AYADrawView.h"

#import "AudioUnitsEngine.h"
struct timbre
{
	//Timbre Storage
	double Amplitude_dB;
	double Detune_cents;
	double m_dDetune_cents2;
	int Octave;
	UINT Osc1Waveform;
	UINT Osc2Waveform;
	UINT Osc3Waveform;
	UINT Osc4Waveform;
	float Osc1Level;
	float Osc2Level;
    float Osc3Level;
	float Osc4Level;
	UINT DetuneSemitones;
	UINT ModMode;
	UINT LegatoMode;
	UINT ResetToZero;
	int PitchBendRange;
	double PortamentoTime_mSec;
	double HSRatio;
	double OscEGIntensity;
	double NoiseOsc_dB;
	double FcControl;
	double QControl;
	double FilterEGIntensity;
	double LFOAmpIntensity;
	double Rate_LFO;
	double OscLFOIntensity;
	double FilterLFOIntensity;
	UINT LFOWaveform;
	double LFOPanIntensity;
	double AttackTime_mSec;
	double DecayReleaseTime_mSec;
	double SustainLevel;
	double DCAEGIntensity;
	UINT FilterKeyTrack;
	double FilterKeyTrackIntensity;
	UINT VelocityToAttackScaling;
	UINT NoteNumberToDecayScaling;
	double PulseWidth_Pct;
	float ArcTanKNeg;
	float ArcTanKPos;
	int Stages;
	UINT InvertStages;
	float Gain;
    
    
};

@interface AYAMainViewController : UIViewController <AYANodeDelegate,UISplitViewControllerDelegate>{
    
    AYAConnectionManager *connectionManager;
    
    AYADrawView *drawView;
    
    __weak IBOutlet UISegmentedControl *modeSelection;
    
    CGPoint previouspoint;
    
//    AYANodeView* startView;
    
    NSMutableArray *tempHitArray;
    
    NSMutableArray *nodes;
    
    NSTimer *m_AnimationTimer;
    
    UIBezierPath *path;
    CGPoint previousPoint;
    
    AudioUnitsEngine *auEngine;
    
    timbre timbre1;
    timbre timbre2;
    
    float m_MIDIFreqTable[128];

    int currentOctave;
    
    //Local Copies of Synth Parameters
    // We need to hold on to a copy of these so that we can update them for the voice
    UINT m_uOsc1Waveform;
	double m_dDetune_cents;
	UINT m_uOsc2Waveform;
    UINT m_uOsc3Waveform;
    UINT m_uOsc4Waveform;
	float m_fOsc1Level;
	float m_fOsc2Level;
	float m_fOsc3Level;
	float m_fOsc4Level;
	UINT m_uDetuneSemitones;
	int m_nOctave;
	double m_dFcControl;
	double m_dRate_LFO;
	double m_dAttackTime_mSec;
	double m_dPulseWidth_Pct;
	double m_dPortamentoTime_mSec;
	double m_dQControl;
	double m_dOscLFOIntensity;
	double m_dDecayReleaseTime_mSec;
	double m_uModMode;
	float m_fFeedback_pct;
	float m_fDelay_ms;
	float m_fWetLevel_pct;
	double m_dHSRatio;
	double m_dOscEGIntensity;
	double m_dFilterEGIntensity;
	double m_dFilterLFOIntensity;
	double m_dSustainLevel;
	float m_fDecay;
	float m_fBandwidth;
	float m_fDamping;
	float m_fWetPct;
	double m_dNoiseOsc_dB;
    double m_dLFOAmpIntensity;
	double m_dLFOPanIntensity;
	UINT m_uLFO_Waveform;
	double m_dDCAEGIntensity;
	float m_fArcTanKPos;
	float m_fArcTanKNeg;
	int m_nStages;
	UINT m_uInvertStages;
	float m_fGain;
	double m_dAmplitude_dB;
	UINT m_uLegatoMode;
	int m_nPitchBendRange;
	UINT m_uResetToZero;
	UINT m_uFilterKeyTrack;
	double m_dFilterKeyTrackIntensity;
	UINT m_uVelocityToAttackScaling;
	UINT m_uNoteNumberToDecayScaling;
	UINT m_uTimbreSelection;
}
-(void)clearGraph;

-(void)saveGraphinSlot:(int)slot;

-(void)loadGraphfromSlot:(int)slot;


@end
