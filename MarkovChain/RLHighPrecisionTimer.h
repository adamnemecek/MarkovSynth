//
//  RLHighPrecisionTimer.h
//  RLCommon
//
//  Created by Chris Bennett on 5/28/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <assert.h>
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>

@protocol RLTimerDelegate <NSObject>

@required
-(void) timerExpired:(double)elapsedMilliseconds;

@end


@interface RLHighPrecisionTimer : NSObject

-(id) initWithInterval:(double)milliseconds andHighPriority:(bool)highPriority andLabel:(NSString *)label andDelegate:(id <RLTimerDelegate>)delegate;
-(void) stop;

@end
