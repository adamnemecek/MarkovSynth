//
//  AYASaveStateViewController.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/24/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "TRPGenericCollectionViewCell.h"

@class AYAMainViewController;

@protocol SaveStateViewControllerDelegate <NSObject>

-(void)saveGraphWithSlotNum:(int)slot;

-(void)loadGraphWithSlotNum:(int)slot;

@end

@interface AYASaveStateViewController : UIViewController <UICollectionViewDelegate, UICollectionViewDataSource>
@property (weak, nonatomic) IBOutlet UICollectionView *collectionView;
@property (weak, nonatomic) id delegate;
@property (strong, nonatomic) IBOutlet UICollectionView *visualEffectView;
@property (weak, nonatomic) AYAMainViewController *mainVC;
@property bool isSaving;
@end
