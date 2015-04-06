//
//  AYANodeView.m
//  Synth2
//
//  Created by Andrew Ayers on 4/7/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYANodeView.h"

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
        knobControlView = [[KnobControlView alloc] initWithFrame:CGRectMake(self.bounds.size.width/2-5,self.bounds.size.height/2-5, 10, 10)];
        [self addSubview:knobControlView];
        
        
        
        self.backgroundColor = [UIColor clearColor];
        backgroundLayer = [[CAGradientLayer alloc] init];
        [backgroundLayer setCornerRadius:self.bounds.size.height/2];
        backgroundLayer.frame = self.bounds;
        
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
        [nodeTapGestureRecognizer addTarget:self action:@selector(recievedEvent)];
        [self addGestureRecognizer:nodeTapGestureRecognizer];
        
        UIPinchGestureRecognizer* pinchGestureRecognizer = [[UIPinchGestureRecognizer alloc] init];
        [pinchGestureRecognizer addTarget:self action:@selector(scaleTheView:)];
        [self addGestureRecognizer:pinchGestureRecognizer];

        
        
        
    }
    return self;
}

-(void)setNoteName:(NSString *)noteName{
    if (_noteName != noteName) {
        _noteName = noteName;
        [_noteNameLayer setString:_noteName];
    }
}

-(void)recievedEvent{
    [knobControlView removeFromSuperview];
    [self.delegate noteOn:self.note];
    float internalNoteLenth = noteLength;
    if (noteLength == -1) {
        int noteLenthSelector = arc4random_uniform(3);
        switch (noteLenthSelector) {
            case 0:
                internalNoteLenth = 1.0f;
                break;
            case 1:
                internalNoteLenth = 0.5f;
                break;
            case 2:
                internalNoteLenth = 0.25f;
                break;
            case 3:
                internalNoteLenth = 0.125f;
                break;
            default:
                break;
        }
    }
    
    self.remainingTickCount = internalNoteLenth * 16;
//    noteTimer = [NSTimer scheduledTimerWithTimeInterval:internalNoteLenth target:self selector:@selector(noteCompleted) userInfo:nil repeats:NO];
    
    CALayer *eventLayer = [[CALayer alloc] init];
    [eventLayer setCornerRadius:self.frame.size.height/2];
    eventLayer.frame = self.bounds;
    eventLayer.backgroundColor = [UIColor colorWithRed:100.0/255.0 green:149.0/255.0 blue:155.0/255.0 alpha:0.3].CGColor;
    CABasicAnimation *eventAnimation = [CABasicAnimation animationWithKeyPath:@"transform"];
    // Add ourselves as the delegate so we get the completion callback
    [eventAnimation setDelegate:self];
    // Set the duration using the number of squares calculated earlier
    [eventAnimation setDuration:internalNoteLenth];
    // Set the from and to values using the cellMatrix
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
    [eventOpacityAnimation setDuration:internalNoteLenth];
    // Set the from and to values using the cellMatrix
    [eventOpacityAnimation setToValue:@(0.0)];
    [eventOpacityAnimation setAutoreverses:NO];
    // Not totally sure, it was in the book
    [eventOpacityAnimation setFillMode:kCAFillModeRemoved];
    [eventOpacityAnimation setRepeatCount:1];
    [eventOpacityAnimation setRemovedOnCompletion:YES];
    
    CAAnimationGroup *group = [[CAAnimationGroup alloc] init];
    [group setDuration:internalNoteLenth];
    [group setRepeatCount:1.0f];
    [group setDelegate:self];
    [group setValue:eventLayer forKey:@"layer"];
    [group setValue:@(self.note) forKeyPath:@"notenumber"];
    group.animations = [NSArray arrayWithObjects:eventAnimation,eventOpacityAnimation, nil];
    [eventLayer addAnimation:group forKey:@"grouppulse"];
    
    
    [self.layer insertSublayer:eventLayer atIndex:0];
    
    
    float totalProbability = 0;
    AYAConnection *selectedConnection;
    for (AYAConnection *connection in connectionArray) {
        totalProbability += roundf(connection.probability*10000);
    }
    int random =  arc4random_uniform(totalProbability);
    for (AYAConnection *connection in connectionArray) {
        if (random<roundf(connection.probability*10000)) {
            selectedConnection = connection;
            break;
        }else{
            random -= roundf(connection.probability*10000);
        }
    }
    
    self.nextView = selectedConnection.endView;

    if ([[selectedConnection.lineLayer valueForKeyPath:@"connectionType"] intValue] == 0) {
        AYANodeView *fromView = selectedConnection.startView;
        AYANodeView *toView = selectedConnection.endView;
        CALayer *pulseLayer = [[CALayer alloc] init];
        [pulseLayer setCornerRadius:25/2];
        pulseLayer.frame = CGRectMake(self.bounds.size.width/2 - 12, self.bounds.size.height/2 - 12, 25, 25);
        pulseLayer.backgroundColor = [UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:0.3].CGColor;
        CABasicAnimation *opacityAnimation = [CABasicAnimation animationWithKeyPath:@"position"];
        // Add ourselves as the delegate so we get the completion callback
        [opacityAnimation setDelegate:self];
        // Set the duration using the number of squares calculated earlier
        [opacityAnimation setDuration:internalNoteLenth];
        // Set the from and to values using the cellMatrix
        [opacityAnimation setToValue:[NSValue valueWithCGPoint:CGPointMake(toView.center.x-fromView.center.x + self.bounds.size.width/2, toView.center.y-fromView.center.y + self.bounds.size.height/2)]];
        [opacityAnimation setAutoreverses:NO];
        // Not totally sure, it was in the book
        [opacityAnimation setFillMode:kCAFillModeRemoved];
        [opacityAnimation setRepeatCount:1];
        // animation also has some places for extra information that lets me know what to do when I call the generic completion method.
        [opacityAnimation setRemovedOnCompletion:YES];
        [opacityAnimation setValue:pulseLayer forKey:@"layer"];
        [opacityAnimation setValue:selectedConnection forKey:@"connection"];
        [pulseLayer addAnimation:opacityAnimation forKey:@"pulse"];
        
        [self.layer addSublayer:pulseLayer];
        pulseLayer = nil;
    }else if([[selectedConnection.lineLayer valueForKeyPath:@"connectionType"] intValue] == 1){
        CALayer *pulseLayer = [[CALayer alloc] init];
        [pulseLayer setCornerRadius:25/2];
        pulseLayer.frame = CGRectMake(12, 12, 25, 25);
        pulseLayer.backgroundColor = [UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:0.3].CGColor;
        CAKeyframeAnimation * theAnimation;
        // Create the animation object, specifying the position property as the key path.
        theAnimation=[CAKeyframeAnimation animationWithKeyPath:@"position"];
        theAnimation.path=((CAShapeLayer*)selectedConnection.lineLayer).path;
        theAnimation.duration=internalNoteLenth;
        [theAnimation setFillMode:kCAFillModeRemoved];
        [theAnimation setRepeatCount:1];
        [theAnimation setDelegate:self];
        theAnimation.calculationMode = kCAAnimationCubicPaced;
        // animation also has some places for extra information that lets me know what to do when I call the generic completion method.
        [theAnimation setRemovedOnCompletion:YES];
        [theAnimation setValue:pulseLayer forKey:@"layer"];
        [theAnimation setValue:selectedConnection forKey:@"connection"];
        [pulseLayer addAnimation:theAnimation forKey:@"pulse"];
        [self.layer addSublayer:pulseLayer];
        pulseLayer = nil;
    }
}

-(void)animationDidStop:(CAAnimation *)anim finished:(BOOL)flag{
    if (flag) {
        [[anim valueForKey:@"layer"] removeAllAnimations];
        [[anim valueForKey:@"layer"] removeFromSuperlayer];
//        if ([anim valueForKey:@"connection"]) {
//            AYAConnection *connection = [anim valueForKey:@"connection"];
//            [connection.endView recievedEvent];
//        }
//        if ([[anim valueForKey:@"notenumber"]intValue]) {
//    //        [self.delegate noteOff:[[anim valueForKey:@"notenumber"]intValue]];
//        }
    }
}

-(void)noteCompleted{
    [self.delegate noteOff:self.note];

}

-(void)scaleTheView:(UIPinchGestureRecognizer*)pinchRecognizer{
    if (pinchRecognizer.scale>2.0) {
        [knobControlView removeFromSuperview];
        knobControlView = [[KnobControlView alloc] initWithFrame:CGRectMake(self.bounds.size.width/2-50,self.bounds.size.height/2-50, 100, 100)];
        [self addSubview:knobControlView];
    }else{
        [knobControlView removeFromSuperview];

    }
}

-(void)tick
{
    self.remainingTickCount--;
    if (self.remainingTickCount == 0) {
        
        // We want to get this work off the timer thread
        dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0), ^{
            [self noteCompleted];
            [self.nextView recievedEvent];
        });

    }
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
