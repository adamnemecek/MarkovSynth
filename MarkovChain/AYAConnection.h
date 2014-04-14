//
//  AYAConnection.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/11/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AYANodeView;

@interface AYAConnection : NSObject

@property AYANodeView* startView;

@property AYANodeView* endView;

@property CALayer *lineLayer;

@property float probability;

@end
