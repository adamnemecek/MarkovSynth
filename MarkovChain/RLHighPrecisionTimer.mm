//
//  RLHighPrecisionTimer.m
//  RLCommon
//
//  Created by Chris Bennett on 5/28/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#import "RLHighPrecisionTimer.h"


@interface RLHighPrecisionTimer()
{
    uint64_t t0;
}

@property (nonatomic) double mIntervalInMilliseconds;
@property (nonatomic) bool mStopper;
@property (nonatomic) NSString *mLabel;
@property (nonatomic) bool mHighPriority;
@property (nonatomic, strong) id <RLTimerDelegate> mDelegate;


@end



static mach_timebase_info_data_t timebase;

@implementation RLHighPrecisionTimer

-(id) initWithInterval:(double)milliseconds andHighPriority:(bool)highPriority andLabel:(NSString *)label andDelegate:(id <RLTimerDelegate>)delegate
{
    self = [super init];
    if(self)
    {
        self.mStopper = false;
        self.mDelegate = delegate;
        self.mLabel = label;
        self.mHighPriority = highPriority;
        self.mIntervalInMilliseconds = milliseconds;
     
        (void) mach_timebase_info(&timebase);
        
        [NSThread detachNewThreadSelector:@selector(workerLoop:) toTarget:self withObject:nil];
    }
    return self;
}

-(void) workerLoop:(NSObject *)data
{
    @try
    {
        double priority = 0.75;
        if(self.mHighPriority)
            priority = 1;
        
        //Set this thread to the highest priority
        [NSThread setThreadPriority:priority];
        
        [NSThread currentThread].name = [NSString stringWithFormat:@"RLHighPrecisionTimer %@", self.mLabel];
        
        //This allows us a window to fire off an elapsed event just before the timer has actually elapsed
        double earlyThreshold = 0.001;
        
        t0 = mach_absolute_time();
        
        //This is the main loop for the timer
        //It loops until someone calls the stop function
        while(!self.mStopper)
        {
            @autoreleasepool
            {
                uint64_t newTime = mach_absolute_time();
                
                double curTime = ((double)(newTime - t0)) * ((double)timebase.numer) / ((double)timebase.denom) / 1000000.0f;
                
                double remaining = self.mIntervalInMilliseconds - curTime;
                
                if(remaining < earlyThreshold)
                {
                    t0 = newTime;
                    //RLLog(@"Elapsed: %f", curTime);
                    if(self.mDelegate)
                        [self.mDelegate timerExpired:curTime];
                }
                else if(remaining > 1.5)
                    [NSThread sleepForTimeInterval:remaining / 2 / 1000];
            }
        }
    }
    @catch(NSException *exception)
    {
    }
}

-(void) stop
{
    self.mStopper = true;
}

@end