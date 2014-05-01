//
//  RLAudioEffect.h
//  RELOAD
//
//  Created by Chris Bennett on 7/10/12.
//  Copyright (c) 2012 University of Miami. All rights reserved.
//

#import <UIKit/UIKit.h>

/** Default sample rate we init to the sample rate in this base class */
static const int defaultSampleRate = 44100;

/** Base class of a RELOAD audio effect */
@interface RLAudioEffect : NSObject

/** Whether the effect should be enabled, can be useful when indexing effects in an array or something */
@property(nonatomic, assign)BOOL enabled;

/** Sample rate to be used by any processing done inside the processAudioFrame method */
@property (nonatomic) int sampleRate;

/** Number of Channels to be input to the plugin */
@property (nonatomic) int inputNumChannels;

/** Number of Channels to be output from the plugin */
@property (nonatomic) int outputNumChannels;

/**
    Prepare plugin to be used, this would include resetting any deley buffers or anything like that. After this function is called, the plugin should be ready to handle audio.
*/
- (void) prepareForPlay;

/**
    Process a STEREO audio sample, ( index 0 is left, 1 is right ) through the audio effect, input and output should be normalized -1.0 to 1.0
    *** Since we take the input and write the output to one location, BE VERY CAREFUL not to overwrite the input values if they are needed later in the function ***
    @param frame - Address where the input (left) is located when called and where the output (left) should be written on completion. Right is at the next index in each case
 */
- (void) processAudioFrameInPlace:(float *)frame;

/**
 
    I don't like this functionality, it doesn't autocomplete. I would like to move this functionality exclusively into a series of overriden setters so that any side effects to setting a property happen inside the overridden setters. 
 
    General function that will allow other classes to manipulate audio effect parameters of subclasses
    @param value - the packaged number you want to set
    @param key - A string description of the parameter to be changed, subclasses should use a list of if statements over the supported keys to cast the NSNumber to its correct primitive type and set it to the internal variable
 */
- (void) setParameterValue:(NSNumber *)value forKey:(NSString *)key;

@end
