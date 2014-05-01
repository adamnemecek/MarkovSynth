//
//  RLBitCrusher.h
//  RELOAD
//
//  Created by Chris Bennett on 7/16/12.
//  Copyright (c) 2012 University of Miami. All rights reserved.
//

#import "RLAudioEffect.h"

@interface RLBitCrusher : RLAudioEffect

/** number of bits to crush, 0 to 16, but no more than 13 is really advisable */
@property (nonatomic) int numBits;


@end
