//
//  AYANoteTimer.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/4/15.
//  Copyright (c) 2015 AyersAudio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RLHighPrecisionTimer.h"
#import "AudioUnitsEngine.h"
#import "AYANoteTimerProtocol.h"




@interface AYANoteTimer : NSObject <RLTimerDelegate>

-(id)initWithTempo:(int)tempo;

@property (nonatomic) NSMutableArray *delegateArray;


@end
