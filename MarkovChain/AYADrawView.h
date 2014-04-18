//
//  AYADrawView.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/17/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface AYADrawView : UIView{
    UIBezierPath *path;
    CGPoint previousPoint;
}

-(void)handleDrawPan:(UIPanGestureRecognizer*)sender;


@end
