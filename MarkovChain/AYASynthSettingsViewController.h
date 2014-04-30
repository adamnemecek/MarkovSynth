//
//  AYASynthSettingsViewController.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/25/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "KnobControlView.h"

#import "AudioUnitsEngine.h"
#import "AYAMainViewController.h"

@interface AYASynthSettingsViewController : UIViewController
{
    __weak IBOutlet KnobControlView *osc1LevelKnob;

    __weak IBOutlet KnobControlView *osc2LeveKnob;
    __weak IBOutlet UISlider *osc1WFSlider;
    __weak IBOutlet UISlider *osc2WFSlider;
    __weak IBOutlet UILabel *osc1WFLabel;
    __weak IBOutlet UILabel *osc2WFLabel;
    
    __weak IBOutlet KnobControlView *filterFCKnob;
    
    __weak IBOutlet KnobControlView *filterQKnob;
    __weak IBOutlet KnobControlView *detuneSemiKnob;
    
    __weak IBOutlet KnobControlView *detuneCentsKnob;

    __weak IBOutlet KnobControlView *attackTimeKnob;
    
    __weak IBOutlet KnobControlView *decayReleaseTimeKnob;
    
    __weak IBOutlet KnobControlView *sustainLevelKnob;
    
    IBOutletCollection(KnobControlView) NSArray *allKnobs;
    
    AudioUnitsEngine *auEngine;
    
}
- (IBAction)wf1sliderchanged:(id)sender;
- (IBAction)wf2sliderChanged:(id)sender;

@property (weak, nonatomic) AYAMainViewController *mainVC;

@end
