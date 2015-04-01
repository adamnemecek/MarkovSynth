//
//  AYAVoiceThread.m
//  MarkovChain
//
//  Created by Chris Bennett on 4/1/15.
//  Copyright (c) 2015 AyersAudio. All rights reserved.
//

#import "AYAVoiceThread.h"

#define MAX_QUEUE_SIZE 9182

@interface AYAVoiceThread (){
    CVoice *mVoice;
    
    double *returnArray;
    
    double *circBuff;
    int producerIndex;
    int consumerIndex;
}
@property (nonatomic) NSMutableArray *mSampleQueue;

@end


@implementation AYAVoiceThread

-(id)initWithVoice:(CVoice *)voice
{
    self = [super init];
    if (self) {
        mVoice = voice;
        
        circBuff = (double *)calloc(MAX_QUEUE_SIZE, sizeof(double));
        producerIndex = 0;
        consumerIndex = 0;
        [NSThread detachNewThreadSelector:@selector(workerLoop) toTarget:self withObject:nil];

        
        
        return self;
    }
    
    return nil;
}



-(void)workerLoop
{
    [NSThread currentThread].name = @"Voice_Thread";
    
    double dLeftOutput;
    double dRightOutput;
    
    while (TRUE) {
        bool full = TRUE;

        if ((producerIndex+1)%MAX_QUEUE_SIZE != consumerIndex && mVoice->m_bNoteOn) {
            full = FALSE;
        }

        
        if (!full) {
            for (int i=0; i<abs(producerIndex-consumerIndex); i++) {
                mVoice->doVoice(dLeftOutput, dRightOutput);
                
                circBuff[producerIndex] = dLeftOutput;
                producerIndex = [self incIndex:producerIndex];
            }
        }
        else
        {
           [NSThread sleepForTimeInterval:0.001];
        }
    }
}

-(int)incIndex:(int)index{
    return (index+1)%MAX_QUEUE_SIZE;
}

-(double)getSample;
{
    bool empty = TRUE;
    

    if ((consumerIndex+1)%MAX_QUEUE_SIZE != producerIndex) {
        empty = FALSE;
    }
    
    double output = 0;
    
    if (!empty) {

            output = circBuff[consumerIndex];
            consumerIndex = [self incIndex:consumerIndex];

    }
    
    return output;
}

@end
