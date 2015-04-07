//
//  AYANodeView.m
//  Synth2
//
//  Created by Andrew Ayers on 4/7/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYANodeView.h"

@interface NoteEvent : NSObject

@property AYANodeView *nextView;

@property int remainingTickCount;

@end

@implementation NoteEvent

@end


@implementation AYANodeView
@synthesize connectionArray,note,backgroundLayer;

@synthesize noteName = _noteName;
@synthesize noteLength;
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        note = 60;
        _noteName = @"C 4";
        noteLength = 0.5;
        // Initialization code

        self.noteEvents = [NSMutableArray array];
        
        self.backgroundColor = [UIColor clearColor];
        backgroundLayer = [[CAGradientLayer alloc] init];
        [backgroundLayer setCornerRadius:self.bounds.size.height/2];
        backgroundLayer.frame = self.bounds;
        
        [self addGradientAnimation];

        connectionArray = [[NSMutableArray alloc] init];
        
        _noteNameLayer = [[CATextLayer alloc] init];
        _noteNameLayer.frame = CGRectMake(self.bounds.size.width/2-10,self.bounds.size.height/2-7, 20, 14);
        [_noteNameLayer setString:_noteName];
        [_noteNameLayer setAlignmentMode:kCAAlignmentCenter];
        [_noteNameLayer setFontSize:12];
        [self.layer addSublayer:_noteNameLayer];
        
        // Single tap recognizer to activate nodes
        UITapGestureRecognizer* nodeTapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
        [nodeTapGestureRecognizer setNumberOfTouchesRequired:1];
        [nodeTapGestureRecognizer setNumberOfTapsRequired:1];
        [nodeTapGestureRecognizer addTarget:self action:@selector(queueEvent)];
        [self addGestureRecognizer:nodeTapGestureRecognizer];
    }
    return self;
}


-(void)queueEvent
{
    NoteEvent *event = [[NoteEvent alloc] init];
    event.nextView = self;
    event.remainingTickCount = 1;
    [self.noteEvents addObject:event];
}

-(void)setNoteName:(NSString *)noteName{
    if (_noteName != noteName) {
        _noteName = noteName;
        [_noteNameLayer setString:_noteName];
    }
}

-(void)recievedEvent{
    [self.delegate noteOn:self.note];
    self.internalNoteLength = noteLength;
    if (noteLength == -1) {
        int noteLenthSelector = arc4random_uniform(3);
        switch (noteLenthSelector) {
            case 0:
                self.internalNoteLength = 1.0f;
                break;
            case 1:
                self.internalNoteLength = 0.5f;
                break;
            case 2:
                self.internalNoteLength = 0.25f;
                break;
            case 3:
                self.internalNoteLength = 0.125f;
                break;
            default:
                break;
        }
    }
    
    
    
    [self addPulseAnimation];
    
    float totalProbability = 0;
    for (AYAConnection *connection in connectionArray) {
        totalProbability += roundf(connection.probability*10000);
    }
    int random =  arc4random_uniform(totalProbability);
    for (AYAConnection *connection in connectionArray) {
        if (random<roundf(connection.probability*10000)) {
            self.selectedConnection = connection;
            break;
        }else{
            random -= roundf(connection.probability*10000);
        }
    }
    
    self.nextView = self.selectedConnection.endView;
    self.remainingTickCount = self.internalNoteLength * 8;
    
    NoteEvent *event = [[NoteEvent alloc] init];
    event.nextView = self.nextView;
    event.remainingTickCount = self.internalNoteLength * 8;
    
    [self.noteEvents addObject:event];
    
    [self addTravelerAnimation];

}

-(void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag{
    if (flag) {
        [[anim valueForKey:@"layer"] removeAllAnimations];
        [[anim valueForKey:@"layer"] removeFromSuperlayer];
    }
}

-(void)noteCompleted{
    [self.delegate noteOff:self.note];
}

-(void)tick
{
    for (NoteEvent* event in self.noteEvents.copy) {
        event.remainingTickCount--;
        if (event.remainingTickCount == 0) {
            
            [self.noteEvents removeObject:event];
            // We want to get this work off the timer thread
            dispatch_async(dispatch_get_main_queue(), ^{
                [self noteCompleted];
                [event.nextView recievedEvent];
            });
            
        }
    }
}
-(void)addTravelerAnimation
{
    AYANodeView *fromView = self;
    AYANodeView *toView = self.nextView;
    CALayer *pulseLayer = [[CALayer alloc] init];
    [pulseLayer setCornerRadius:25/2];
    pulseLayer.frame = CGRectMake(self.bounds.size.width/2 - 12, self.bounds.size.height/2 - 12, 25, 25);
    pulseLayer.backgroundColor = [UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:0.3].CGColor;

    CAAnimation *theAnimation;
    
     if ([[self.selectedConnection.lineLayer valueForKeyPath:@"connectionType"] intValue] == 0) {
        theAnimation = [CABasicAnimation animationWithKeyPath:@"position"];
        [(CABasicAnimation*)theAnimation setToValue:[NSValue valueWithCGPoint:CGPointMake(toView.center.x-fromView.center.x + self.bounds.size.width/2, toView.center.y-fromView.center.y + self.bounds.size.height/2)]];
        [theAnimation setAutoreverses:NO];
    }else if([[self.selectedConnection.lineLayer valueForKeyPath:@"connectionType"] intValue] == 1){
        theAnimation=[CAKeyframeAnimation animationWithKeyPath:@"position"];
        
        ((CAKeyframeAnimation*)theAnimation).path=((CAShapeLayer*)self.selectedConnection.lineLayer).path;
        ((CAKeyframeAnimation*)theAnimation).calculationMode = kCAAnimationCubicPaced;
        [self.layer addSublayer:pulseLayer];
    }
    
    [theAnimation setDelegate:self];
    [theAnimation setDuration:self.internalNoteLength];
    [theAnimation setFillMode:kCAFillModeRemoved];
    [theAnimation setRepeatCount:1];
    [theAnimation setRemovedOnCompletion:YES];
    [theAnimation setValue:pulseLayer forKey:@"layer"];
    [theAnimation setValue:self.selectedConnection forKey:@"connection"];
    [pulseLayer addAnimation:theAnimation forKey:@"pulse"];
    [self.layer addSublayer:pulseLayer];

}

-(void)addGradientAnimation
{
    UIColor *colorOne = [UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0];
    UIColor *colorTwo = [UIColor colorWithRed:255.0/255.0 green:94.0/255.0 blue:58.0/255.0 alpha:1.0];
    NSArray *colors =  [NSArray arrayWithObjects:(id)colorOne.CGColor, colorTwo.CGColor,nil];
    
    NSNumber *stopOne = [NSNumber numberWithFloat:1.0];
    NSNumber *stopTwo = [NSNumber numberWithFloat:0.6];
    NSArray *locations = [NSArray arrayWithObjects:stopOne, stopTwo, nil];
    
    backgroundLayer.colors = colors;
    backgroundLayer.locations = locations;
    
    CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"colors"];
    animation.duration = 5.0f;
    animation.delegate =self;
    animation.fromValue = (CAGradientLayer *)backgroundLayer.colors;
    animation.toValue = [NSArray arrayWithObjects:(id)colorTwo.CGColor, colorOne.CGColor,nil];
    [animation setAutoreverses:YES];
    [animation setRepeatCount:10000];
    [backgroundLayer addAnimation:animation forKey:@"animateColors"];
    
    [backgroundLayer setBackgroundColor:[UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0].CGColor];
    
    [self.layer addSublayer:backgroundLayer];
}

-(void)addPulseAnimation
{
    CALayer *eventLayer = [[CALayer alloc] init];
    [eventLayer setCornerRadius:self.frame.size.height/2];
    eventLayer.frame = self.bounds;
    eventLayer.backgroundColor = [UIColor colorWithRed:100.0/255.0 green:149.0/255.0 blue:155.0/255.0 alpha:0.3].CGColor;
    CABasicAnimation *eventAnimation = [CABasicAnimation animationWithKeyPath:@"transform"];
    // Add ourselves as the delegate so we get the completion callback
    [eventAnimation setDelegate:self];
    [eventAnimation setDuration:self.internalNoteLength];
    [eventAnimation setToValue:[NSValue valueWithCATransform3D:CATransform3DMakeScale(2.0, 2.0, 1)]];
    [eventAnimation setAutoreverses:NO];
    // Not totally sure, it was in the book
    [eventAnimation setFillMode:kCAFillModeRemoved];
    [eventAnimation setRepeatCount:1];
    [eventAnimation setRemovedOnCompletion:YES];
    
    CABasicAnimation *eventOpacityAnimation = [CABasicAnimation animationWithKeyPath:@"opacity"];
    // Add ourselves as the delegate so we get the completion callback
    [eventOpacityAnimation setDelegate:self];
    // Set the duration using the number of squares calculated earlier
    [eventOpacityAnimation setDuration:self.internalNoteLength];
    // Set the from and to values using the cellMatrix
    [eventOpacityAnimation setToValue:@(0.0)];
    [eventOpacityAnimation setAutoreverses:NO];
    // Not totally sure, it was in the book
    [eventOpacityAnimation setFillMode:kCAFillModeRemoved];
    [eventOpacityAnimation setRepeatCount:1];
    [eventOpacityAnimation setRemovedOnCompletion:YES];
    
    CAAnimationGroup *group = [[CAAnimationGroup alloc] init];
    [group setDuration:self.internalNoteLength];
    [group setRepeatCount:1.0f];
    [group setDelegate:self];
    [group setValue:eventLayer forKey:@"layer"];
    [group setValue:@(self.note) forKeyPath:@"notenumber"];
    group.animations = [NSArray arrayWithObjects:eventAnimation,eventOpacityAnimation, nil];
    [eventLayer addAnimation:group forKey:@"grouppulse"];
    
    
    [self.layer insertSublayer:eventLayer atIndex:0];
}


@end
