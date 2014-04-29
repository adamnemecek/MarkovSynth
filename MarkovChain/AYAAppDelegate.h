//
//  AYAAppDelegate.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/10/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AYAMainViewController.h"
#import <MSDynamicsDrawerViewController/MSDynamicsDrawerViewController.h>
#import "TRPLeftDrawerViewController.h"



@interface AYAAppDelegate : UIResponder <UIApplicationDelegate,MSDynamicsDrawerViewControllerDelegate>

@property (strong, nonatomic) UIWindow *window;
@property (strong, nonatomic) MSDynamicsDrawerViewController *rootViewController;
@property (strong, nonatomic) TRPLeftDrawerViewController *leftDrawerViewController;
@property (strong, nonatomic) AYAMainViewController *mainVC;
@property (strong, nonatomic) UISplitViewController *splitViewController;



@end
