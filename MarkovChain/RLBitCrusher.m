//
//  RLBitCrusher.m
//  RELOAD
//
//  Created by Chris Bennett on 7/16/12.
//  Copyright (c) 2012 University of Miami. All rights reserved.
//

#import "RLBitCrusher.h"

@implementation RLBitCrusher

@synthesize numBits;

- (id)init 
{
    if((self = [super init])) {
        numBits = 3;
    }
    return self;
}

- (void) processAudioFrameInPlace:(float *)frame
{
    int rawLeft = (int)(frame[0] * 32768.0);
    int rawRight = (int)(frame[1] * 32768.0);
    
    int subLeft = abs(rawLeft) % (int)pow(2, numBits);
    int subRight = abs(rawRight) % (int)pow(2, numBits);

    if (rawLeft < 0) {
        rawLeft += subLeft;
    }
    else {
        rawLeft -= subLeft;
    }
    if (rawRight < 0) {
        rawRight += subRight;
    }
    else {
        rawRight -= subRight;
    }
    
    frame[0] = (float)rawLeft/32768.0;
    frame[1] = (float)rawRight/32768.0;
    
}

-(void)prepareForPlay{
    //Need to override, but nothing do to here. 
}


- (void) setParameterValue:(NSNumber *)value forKey:(NSString *)key
{
    if ( [ key isEqualToString:@"numBitsCrushed" ] )
    {
//        [self setCrushBits:[value shortValue]];
        
        if([value intValue] > 15)
            self.numBits = 15;
        else if([value intValue] < 0)
            self.numBits = 0;
        else {
            self.numBits = [value intValue];
        }
//        printf("%d \n", numBits);
    }
    
}

@end
