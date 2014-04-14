//
//  AYAConnectionManager.m
//  Synth2
//
//  Created by Andrew Ayers on 4/7/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYAConnectionManager.h"

@implementation AYAConnectionManager

@synthesize connectionArray;

-(id)init{
    self = [super init];
    if (self) {
        connectionArray = [[NSMutableArray alloc]  init];
    }
    
    return self;
    
}

@end
