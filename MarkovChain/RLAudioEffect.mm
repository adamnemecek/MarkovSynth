//
//  RLAudioEffect.m
//  RELOAD
//
//  Created by Chris Bennett on 7/10/12.
//  Copyright (c) 2012 University of Miami. All rights reserved.
//

#import "RLAudioEffect.h"

@implementation RLAudioEffect

@synthesize enabled;
@synthesize sampleRate;
@synthesize inputNumChannels;
@synthesize outputNumChannels;

- (id)init 
{
	if((self = [super init])) {
		enabled = YES;
        sampleRate = defaultSampleRate;
	}
	return self;
}
- (void) prepareForPlay{
    // Default is to do nothing -- so log an error!
	NSLog(@"ERROR: Attempting to use base audio effect!");
    [NSException raise:NSInternalInconsistencyException
                format:@"You must override %@ in a subclass", NSStringFromSelector(_cmd)];
}

- (void) processAudioFrameInPlace:(float *)frame
{
	// Default is to do nothing -- so log an error!
	NSLog(@"ERROR: Attempting to use base audio effect!");
    [NSException raise:NSInternalInconsistencyException
                format:@"You must override %@ in a subclass", NSStringFromSelector(_cmd)];
}

- (void) setParameterValue:(NSNumber *)value forKey:(NSString *)key
{
    // Default is to do nothing -- so log an error!
	NSLog(@"ERROR: Attempting to set Value on audio effect base class!");
    [NSException raise:NSInternalInconsistencyException
                format:@"You must override %@ in a subclass", NSStringFromSelector(_cmd)];
}

@end
