//
//  TouchDrawingView.m
//  ReLOADWireframe
//
//  Created by Andrew Ayers on 1/31/14.
//  Copyright (c) 2014 Rob Rehrig. All rights reserved.
//

#import "TouchDrawingView.h"
@implementation TouchDrawingView

{
    UIBezierPath *path;
    UIImage *incrementalImage;
    CGPoint pts[5]; // we now need to keep track of the four points of a Bezier segment and the first control point of the next segment
    uint ctr;
    CGPoint point1;
    CGPoint point2;
    CGPoint point3;
    int numtaps;
}

@synthesize angle;
@synthesize delegate;

- (id)initWithCoder:(NSCoder *)aDecoder
{
    if (self = [super initWithCoder:aDecoder])
    {
        [self setMultipleTouchEnabled:NO];
        [self setBackgroundColor:[UIColor clearColor]];
        path = [UIBezierPath bezierPath];
        [path setLineWidth:3.0];
    }
    return self;
}
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self setMultipleTouchEnabled:NO];
        path = [UIBezierPath bezierPath];
        [path setLineWidth:2.0];
        numtaps = 0;
        angle = 0;
        
    }
    return self;
}

-(void)clearBitmap{
    incrementalImage = nil;
}
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    [[UIColor redColor] setStroke];
    [incrementalImage drawInRect:rect];
    [path stroke];
    
    [[UIColor greenColor] setStroke];
    if (numtaps == 3) {
        CGContextRef ctx = UIGraphicsGetCurrentContext();
        CGContextSetLineWidth(ctx, 2.0f);
        CGContextMoveToPoint(ctx, point1.x, point1.y);
        CGContextAddLineToPoint(ctx, point2.x, point2.y);
        CGContextMoveToPoint(ctx, point2.x, point2.y);
        CGContextAddLineToPoint(ctx, point3.x, point3.y);
        CGContextStrokePath(ctx);
    }

    

}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    ctr = 0;
    UITouch *touch = [touches anyObject];
    pts[0] = [touch locationInView:self];
}
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
    UITouch *touch = [touches anyObject];
    CGPoint p = [touch locationInView:self];
    ctr++;
    pts[ctr] = p;
    if (ctr == 4)
    {
        pts[3] = CGPointMake((pts[2].x + pts[4].x)/2.0, (pts[2].y + pts[4].y)/2.0); // move the endpoint to the middle of the line joining the second control point of the first Bezier segment and the first control point of the second Bezier segment
        [path moveToPoint:pts[0]];
        [path addCurveToPoint:pts[3] controlPoint1:pts[1] controlPoint2:pts[2]]; // add a cubic Bezier from pt[0] to pt[3], with control points pt[1] and pt[2]
        [self setNeedsDisplay];
        // replace points and get ready to handle the next segment
        pts[0] = pts[3];
        pts[1] = pts[4];
        ctr = 1;
    }
}
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self drawBitmap];
    [self setNeedsDisplay];
    [path removeAllPoints];
    ctr = 0;
        for (UITouch *aTouch in touches) {
            if (aTouch.tapCount == 1) {
                // The view responds to the tap
                [self respondToTapGesture:aTouch];
            } 
        }
}
-(void) respondToTapGesture:(UITouch *)aTouch{
    numtaps++;
    switch (numtaps) {
        case 1:
            point1 = [aTouch locationInView:self];
            break;
        case 2:
            point2 = [aTouch locationInView:self];
            break;
        case 3:
            point3 = [aTouch locationInView:self];
            [self calculateAngle];
            break;
        case 4:
            numtaps = 0;
            break;
            
        default:
            break;
    }
    
}

-(void) calculateAngle{
    double length_21 = sqrt(pow((point2.x - point1.x),2)+pow((point2.y - point1.y),2));
    double length_23 = sqrt(pow((point2.x - point3.x),2)+pow((point2.y - point3.y),2));
    double length_13 = sqrt(pow((point1.x - point3.x),2)+pow((point1.y - point3.y),2));
    angle = acos((pow(length_21,2)+pow(length_23,2)-pow(length_13,2))/(2*length_21*length_23));
    angle = angle/M_PI * 180.00;
    [self distributeAngle];
}

-(void) distributeAngle{
    if ([delegate respondsToSelector:@selector(angleChanged)]) {
        [delegate angleChanged];
    }
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touchesEnded:touches withEvent:event];
}



- (void)drawBitmap
{
    UIGraphicsBeginImageContextWithOptions(self.bounds.size, NO, 0.0);
    if (!incrementalImage) // first time; paint background white
    {
        UIBezierPath *rectpath = [UIBezierPath bezierPathWithRect:self.bounds];
        [[UIColor clearColor] setFill];
        [rectpath fill];
    }
    [incrementalImage drawAtPoint:CGPointZero];
    [[UIColor redColor] setStroke];
    [path stroke];
    incrementalImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
}
@end

