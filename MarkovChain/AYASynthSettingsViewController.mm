//
//  AYASynthSettingsViewController.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/25/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYASynthSettingsViewController.h"

@interface AYASynthSettingsViewController ()

@end

@implementation AYASynthSettingsViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Back" style:UIBarButtonItemStylePlain target:self action:@selector(selfDismissed)];
    for (KnobControlView *kview in allKnobs) {
        [kview init];
        [kview._knobControl addTarget:self
                               action:@selector(handleValueChanged:)
                     forControlEvents:UIControlEventValueChanged];
    }
    auEngine = self.mainVC.auEngine;
    
    
    
    osc1LevelKnob._knobControl.minimumValue = 0.0;
    osc2LeveKnob._knobControl.minimumValue = 0.0;
    osc1LevelKnob._knobControl.maximumValue = 100.0;
    osc2LeveKnob._knobControl.maximumValue = 100.0;
    filterFCKnob._knobControl.minimumValue = 200.0;
    filterFCKnob._knobControl.maximumValue = 20000.0;
    filterQKnob._knobControl.minimumValue = 0.0;
    filterQKnob._knobControl.maximumValue = 10.0;
    detuneCentsKnob._knobControl.maximumValue = 20.0;
    detuneCentsKnob._knobControl.minimumValue = 0.0;
    detuneSemiKnob._knobControl.minimumValue = -12;
    detuneSemiKnob._knobControl.maximumValue = 12;
    attackTimeKnob._knobControl.minimumValue = 100.0;
    attackTimeKnob._knobControl.maximumValue = 500.0;
    decayReleaseTimeKnob._knobControl.minimumValue = 100.0;
    decayReleaseTimeKnob._knobControl.maximumValue = 500.0;
    sustainLevelKnob._knobControl.minimumValue = 0.0;
    sustainLevelKnob._knobControl.maximumValue = 1.0;
    
    CMiniSynthVoice *pVoice = auEngine.auTrack_1_PlaybackInfo.m_VoicePtrStack1[0];

    [osc1LevelKnob._knobControl setValue:pVoice->m_fOsc1Level];
    [osc2LeveKnob._knobControl setValue:pVoice->m_fOsc2Level];
    
}

-(void)updateMiniSynth
{
    CMiniSynthVoice* pVoice;
    for(int i=0; i<16; i++)
    {
        pVoice = auEngine.auTrack_1_PlaybackInfo.m_VoicePtrStack1[i];
        
        pVoice->setDetuneCents(roundf(detuneCentsKnob._knobControl.value));
        pVoice->setOscDetuneSemitones(roundf(detuneSemiKnob._knobControl.value));

        pVoice->setOsc1Waveform(roundf(osc1WFSlider.value));
        pVoice->setOsc2Waveform(roundf(osc2WFSlider.value));
        
        pVoice->setOsc1Level(osc1LevelKnob._knobControl.value);
        pVoice->setOsc2Level(osc2LeveKnob._knobControl.value);

        pVoice->setFilter1Cutoff(filterFCKnob._knobControl.value);
        pVoice->setFilter1Q(filterQKnob._knobControl.value);

        pVoice->setEG1AttackTime_mSec(attackTimeKnob._knobControl.value);

        pVoice->setEG1DecayTime_mSec(decayReleaseTimeKnob._knobControl.value);

        pVoice->setEG1ReleaseTime_mSec(decayReleaseTimeKnob._knobControl.value);

        pVoice->setEG1SustainLevel(sustainLevelKnob._knobControl.value);

        pVoice->update();
    }
}


-(void)handleValueChanged:(id)sender{
    [self updateMiniSynth];
}

-(void)selfDismissed{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)wf1sliderchanged:(id)sender {
    [self updateMiniSynth];
}

- (IBAction)wf2sliderChanged:(id)sender {
    [self updateMiniSynth];
}
@end
