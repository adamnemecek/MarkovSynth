//
//  AYAMidiToMarkovCalc.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/25/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AYAMidiParser.h"

@interface AYAMidiToMarkovCalc : NSObject

+(NSMutableDictionary *)calcProbabilityForFile:(NSString *)filename;

@end
