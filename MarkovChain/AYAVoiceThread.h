//
//  AYAVoiceThread.h
//  MarkovChain
//
//  Created by Chris Bennett on 4/1/15.
//  Copyright (c) 2015 AyersAudio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Voice.h"
#include <queue>
@interface AYAVoiceThread : NSObject

-(id)initWithVoice:(CVoice *)voice;

-(double)getSample;

@end
    