//
//  AYANodeView.h
//  Synth2
//
//  Created by Andrew Ayers on 4/7/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AYAConnection.h"
#import "KnobControlView.h"
@protocol AYANodeDelegate <NSObject>

-(void)noteOn:(int)notenumber;
-(void)noteOff:(int)notenumber;

@end


@interface AYANodeView : UIView{
    KnobControlView *knobControlView;
    
    NSTimer *noteTimer;
}

@property (strong, nonatomic) NSMutableArray *connectionArray;

@property int note;

@property (strong, nonatomic) NSString* noteName;

@property (strong, nonatomic) CAGradientLayer* backgroundLayer;

@property (strong, nonatomic) CATextLayer *noteNameLayer;

@property (weak, nonatomic) id <AYANodeDelegate> delegate;

-(void)recievedEvent;

@end
