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
    
    
    wfNames = @[@"Sine",@"Saw1",@"Saw2",@"Saw3",@"Tri",@"Square",@"Noise",@"PNoise"];
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


    [osc1LevelKnob._knobControl setValue:self.mainVC->m_fOsc1Level];
    [osc2LeveKnob._knobControl setValue:self.mainVC->m_fOsc2Level];
    [filterFCKnob._knobControl setValue:self.mainVC->m_dFcControl];
    [filterQKnob._knobControl setValue:self.mainVC->m_dQControl];
    [detuneCentsKnob._knobControl setContinuous:self.mainVC->m_dDetune_cents];
    [detuneSemiKnob._knobControl setValue:self.mainVC->m_uDetuneSemitones];
    [attackTimeKnob._knobControl setValue:self.mainVC->m_dAttackTime_mSec];
    [decayReleaseTimeKnob._knobControl setValue:self.mainVC->m_dDecayReleaseTime_mSec];
    [sustainLevelKnob._knobControl setValue:self.mainVC->m_dSustainLevel];
    [osc1WFSlider setValue:self.mainVC->m_uOsc1Waveform];
    [osc2WFSlider setValue:self.mainVC->m_uOsc2Waveform];
    [osc1WFLabel setText:wfNames[(int)(roundf(osc1WFSlider.value))]];
    [osc2WFLabel setText:wfNames[(int)(roundf(osc2WFSlider.value))]];
    
}

-(void)updateMiniSynth
{
    
        self.mainVC->m_dDetune_cents = (roundf(detuneCentsKnob._knobControl.value));
        self.mainVC->m_uDetuneSemitones = (roundf(detuneSemiKnob._knobControl.value));

        self.mainVC->m_uOsc1Waveform = (roundf(osc1WFSlider.value));
        self.mainVC->m_uOsc2Waveform = (roundf(osc2WFSlider.value));
    
        [osc1WFLabel setText:wfNames[(int)(roundf(osc1WFSlider.value))]];
        [osc2WFLabel setText:wfNames[(int)(roundf(osc2WFSlider.value))]];

        self.mainVC->m_fOsc1Level = (osc1LevelKnob._knobControl.value);
        self.mainVC->m_fOsc2Level = (osc2LeveKnob._knobControl.value);

        self.mainVC->m_dFcControl = (filterFCKnob._knobControl.value);
        self.mainVC->m_dQControl = (filterQKnob._knobControl.value);

        self.mainVC ->m_dAttackTime_mSec = (attackTimeKnob._knobControl.value);

        self.mainVC-> m_dDecayReleaseTime_mSec = (decayReleaseTimeKnob._knobControl.value);

        self.mainVC -> m_dSustainLevel = (sustainLevelKnob._knobControl.value);

    [self.mainVC updateMiniSynth];
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
