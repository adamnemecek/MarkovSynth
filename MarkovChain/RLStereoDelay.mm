//
//  RLStereoDelay.m
//  RELOAD
//
//  Created by Chris Bennett on 7/16/12.
//  Copyright (c) 2012 University of Miami. All rights reserved.
//

#import "RLStereoDelay.h"

@implementation RLStereoDelay

@synthesize delay_ms;
@synthesize feedback_norm;
@synthesize wetMix_norm;
@synthesize insertEffectsArray;

- (id)init
{
    self = [super init];
    if (self) {

        bufferSize = self.sampleRate * 2;
        
        pBufferL = new float[bufferSize];
        pBufferR = new float[bufferSize];
        
        delay_ms = 0.0;
        feedback_norm = 0.0;
        wetMix_norm = 0.0;
        insertEffectsArray = [[NSMutableArray alloc] init];
        
        [self resetDelay];
        
    }
    return self;
}

-(void)prepareForPlay{
    [self resetDelay];
    [self cookVariables];
}

- (void) resetDelay
{
	// flush buffer
	if(pBufferL)
		memset(pBufferL, 0, bufferSize*sizeof(float));
    if(pBufferR)
		memset(pBufferR, 0, bufferSize*sizeof(float));
    
	// init read/write indices
	writeIndex = 0; // reset the Write index to top
	readIndex = 0; // reset the Write index to top
    
	[self cookVariables];
}

inline float dLinTerp(float x1, float x2, float y1, float y2, float x)
{
	float denom = x2 - x1;
	if(denom == 0)
		return y1; // should not ever happen
    
	// calculate decimal position of x
	float dx = (x - x1)/(x2 - x1);
    
	// use weighted sum method of interpolating
	float result = dx*y2 + (1-dx)*y1;
    
	return result;
    
}

- (void) cookVariables
{
	fDelayInSamples = delay_ms*((float)self.sampleRate/1000.0);
    
	// subtract to make read index
	readIndex = writeIndex - (int)fDelayInSamples;
    
	//  the check and wrap BACKWARDS if the index is negative
	if (readIndex < 0)
		readIndex += bufferSize;	// amount of wrap is Read + Length
    
}

- (void) readDelayLeft:(float *)pLeft andRight:(float *)pRight
{
	// Read the output of the delay at m_nReadIndex
	float yn_L = pBufferL[readIndex];
	float yn_R = pBufferR[readIndex];
	
	// Read the location ONE BEHIND yn at y(n-1)
	int nReadIndex_1 = readIndex - 1;
	if(nReadIndex_1 < 0)
		nReadIndex_1 = bufferSize-1; // m_nBufferSize-1 is last location
    
	// get y(n-1)
	float yn_1_L = pBufferL[nReadIndex_1];
    float yn_1_R = pBufferR[nReadIndex_1];
    
	// interpolate: (0, yn) and (1, yn_1) by the amount fracDelay
	float fFracDelay = fDelayInSamples - (int)fDelayInSamples;
    
	*pLeft = dLinTerp(0, 1, yn_L, yn_1_L, fFracDelay); // interp frac between them
    *pRight = dLinTerp(0, 1, yn_R, yn_1_R, fFracDelay); // interp frac between them
}

- (void) writeDelayAndIncLeft:(float)xn_L andRight:(float)xn_R
{
	// write to the delay line
	pBufferL[writeIndex] = xn_L; // external feedback sample
    pBufferR[writeIndex] = xn_R; // external feedback sample
    
	// incremnent the pointers and wrap if necessary
	writeIndex++;
	if(writeIndex >= bufferSize)
		writeIndex = 0;
    
	readIndex++;
	if(readIndex >= bufferSize)
		readIndex = 0;
}

- (void) processAudioFrameInPlace:(float *)frame
{
    float xn_L = frame[0];
    float xn_R = frame[1];
    
    float yn_L = 0;
    float yn_R = 0;
    
    [self readDelayLeft:&yn_L andRight:&yn_R];
    
	// read delayed output
	yn_L = fDelayInSamples == 0 ? xn_L : yn_L;
    yn_R = fDelayInSamples == 0 ? xn_R : yn_R;
    
	// write to the delay line
    
    float delayWriteVal[2];

    delayWriteVal[0] = xn_L + feedback_norm*yn_L;
    delayWriteVal[1] = xn_R + feedback_norm*yn_R;
    
    if (insertEffectsArray.count == 0) {
        [self writeDelayAndIncLeft:delayWriteVal[0] andRight:delayWriteVal[1]];
    }
    else {
        for (RLAudioEffect *effect in insertEffectsArray) {
            [effect processAudioFrameInPlace:delayWriteVal];
        }
        [self writeDelayAndIncLeft:delayWriteVal[0] andRight:delayWriteVal[1]];
    }
    
	// output attenuation
    frame[0] = wetMix_norm*yn_L + (1.0 - wetMix_norm )*xn_L;
    frame[1] = wetMix_norm*yn_R + (1.0 - wetMix_norm )*xn_R;
    
}

- (void) setParameterValue:(NSNumber *)value forKey:(NSString *)key
{
    if( [key isEqualToString:@"delay_ms"] ){
        delay_ms = [value floatValue];
        [self cookVariables];
    }
    if ([key isEqualToString:@"feedback_norm"]) {
        feedback_norm = [value floatValue];
        
        feedback_norm = feedback_norm < 0 ? 0 : feedback_norm;
        feedback_norm = feedback_norm > 1 ? 1 : feedback_norm;
        
        [self cookVariables];
    }
    if ([key isEqualToString:@"wetMix_norm"]) {
        wetMix_norm = [value floatValue];
    }
}

@end
