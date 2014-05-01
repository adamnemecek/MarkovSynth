//
//  TRPLeftDrawerViewController.h
//  Tripster
//
//  Created by Brian Gerstle on 3/23/14.
//  Copyright (c) 2014 UMiami. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AYAMainViewController.h"
#import "AYASaveStateViewController.h"
#import "AYASynthSettingsViewController.h"

#import "AYAMidiToMarkovCalc.h"
#import "RLStereoDelay.h"

@interface TRPLeftDrawerViewController : UITableViewController <SaveStateViewControllerDelegate>

@property (weak,nonatomic) AYAMainViewController* mainVC;

@end
