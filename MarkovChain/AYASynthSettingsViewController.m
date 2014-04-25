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
}

-(void)handleValueChanged:(id)sender{
    
}

-(void)selfDismissed{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
