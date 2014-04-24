//
//  AYADrawView.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/17/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYADrawView.h"

@implementation AYADrawView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        self.backgroundColor = [UIColor clearColor];
        path = [[UIBezierPath alloc] init];
    }
    return self;
}
-(void)handleDrawPan:(UIPanGestureRecognizer*)sender{
    CGPoint currentPoint = [sender locationInView:self];
    CGPoint midPoint =[self midpointFromPoint:previousPoint andPoint:currentPoint];
    
    if (sender.state == UIGestureRecognizerStateBegan) {
        [path moveToPoint:currentPoint];
    } else if (sender.state == UIGestureRecognizerStateChanged) {
        [path addQuadCurveToPoint:midPoint controlPoint:previousPoint];
    } else if (sender.state == UIGestureRecognizerStateEnded) {
        [path removeAllPoints];
        
    }
    
    previousPoint = currentPoint;
    
    [self setNeedsDisplay];
    
}
-(CGPoint) midpointFromPoint:(CGPoint)p0 andPoint:(CGPoint) p1
{
    CGPoint point = CGPointMake((p0.x + p1.x) / 2.0, (p0.y + p1.y) / 2.0);
    return point;
}
- (void)drawRect:(CGRect)rect
{
    [path setLineWidth:5.0f];
    [[UIColor redColor] setStroke];
    [path stroke];
}


/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
