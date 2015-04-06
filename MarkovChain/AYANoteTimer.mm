//
//  AYANoteTimer.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/4/15.
//  Copyright (c) 2015 AyersAudio. All rights reserved.
//

#import "AYANoteTimer.h"

@interface AYANoteTimer()

@property (nonatomic) int tempo;

@property RLHighPrecisionTimer *timer;


@end

@implementation AYANoteTimer

-(id)initWithTempo:(int)tempo andAUEngine:(AudioUnitsEngine *)auEngine
{

    self = [super init];
    if (self) {
        self.timer = [[RLHighPrecisionTimer alloc] initWithInterval:125 andHighPriority:true andLabel:@"noteTimer" andDelegate:self];
    }
    
    return self;
}

-(void)timerExpired:(double)elapsedMilliseconds
{
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
        for (id<AYANoteTimerDelegate> delegate in self.delegateArray) {
            [delegate tick];
        }
    });
}


@end
