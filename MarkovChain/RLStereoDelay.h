//
//  RLStereoDelay.h
//  RELOAD
//
//  Created by Chris Bennett on 7/16/12.
//  Copyright (c) 2012 University of Miami. All rights reserved.
//

#import "RLAudioEffect.h"

@interface RLStereoDelay : RLAudioEffect
{
    /** delay samples, but fractional based on the millisecond delay value set */
    float fDelayInSamples;
    
    /** pointer to left buffer */
    float *pBufferL;
    
    /** pointer to right buffer */
    float *pBufferR;
    
    // size in samples
    int bufferSize;
    
    int readIndex;
	int writeIndex;;
    
}

/** Delay time milliseconds */
@property (nonatomic) float delay_ms;

/** Normalized (0 to 1) feedback percentage */
@property (nonatomic) float feedback_norm;

/** Normatlized (0 to 1) wetMix, compliments the dryMix implicity */
@property (nonatomic) float wetMix_norm;

/** Array of RLAudioEffects to be added into the feedback path, if not desired, don't mess with it */
@property (nonatomic) NSMutableArray *insertEffectsArray;

/** Call to reset delay buffer and stuff */
- (void) resetDelay;

@end
