//
//  AYANodeView.h
//  Synth2
//
//  Created by Andrew Ayers on 4/7/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AYAConnection.h"

@interface AYANodeView : UIView

@property (strong, nonatomic) NSMutableArray *connectionArray;

@property int note;

@property (strong, nonatomic) NSString* noteName;

@property (strong, nonatomic) CAGradientLayer* backgroundLayer;

@property (strong, nonatomic) CATextLayer *noteNameLayer;

-(void)recievedEvent;

@end
