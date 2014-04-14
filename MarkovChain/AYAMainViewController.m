//
//  AYAMainViewController.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/10/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYAMainViewController.h"

typedef NS_ENUM(NSInteger, connectionType){
    kConnectionTypeLine,
    kConnectionTypeCircle,
};

@interface AYAMainViewController ()

@end

@implementation AYAMainViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    connectionManager = [[AYAConnectionManager alloc] init];
    
    nodes = [[NSMutableArray alloc] init];
    CAGradientLayer *backgroundLayer = [[CAGradientLayer alloc] init];
    backgroundLayer.frame = self.view.bounds;
    
    UIColor *colorOne = [UIColor colorWithRed:74.0/255.0 green:74.0/255.0 blue:74.0/255.0 alpha:1.0];
    UIColor *colorTwo = [UIColor colorWithRed:43.0/255.0 green:43.0/255.0 blue:43.0/255.0 alpha:1.0];
    NSArray *colors =  [NSArray arrayWithObjects:(id)colorOne.CGColor, colorTwo.CGColor,nil];
    
    NSNumber *stopOne = [NSNumber numberWithFloat:0.1];
    NSNumber *stopTwo = [NSNumber numberWithFloat:0.8];
    NSArray *locations = [NSArray arrayWithObjects:stopOne, stopTwo, nil];
    
    backgroundLayer.colors = colors;
    backgroundLayer.locations = locations;
//    [self.view.layer addSublayer:backgroundLayer];
    
    [self.view bringSubviewToFront:_tableView];
    [self.view bringSubviewToFront:modeSelection];
    
    // Do any additional setup after loading the view from its nib.
}

-(void)viewDidAppear:(BOOL)animated{
    UITapGestureRecognizer* tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
    [tapGestureRecognizer setNumberOfTouchesRequired:1];
    [tapGestureRecognizer setNumberOfTapsRequired:2];
    [tapGestureRecognizer addTarget:self action:@selector(handleTap:)];
    [self.view addGestureRecognizer:tapGestureRecognizer];
    
    UIPanGestureRecognizer* panGestureRecognizer = [[UIPanGestureRecognizer alloc] init];
    [panGestureRecognizer setMinimumNumberOfTouches:1];
    [panGestureRecognizer setMaximumNumberOfTouches:1];
    [panGestureRecognizer addTarget:self action:@selector(handlePan:)];
    [self.view addGestureRecognizer:panGestureRecognizer];
}

-(void)handleTap:(UITapGestureRecognizer*)sender{
    NSLog(@"TAPTAP");
    if (modeSelection.selectedSegmentIndex == 0) {
        if ([nodes count]==0) {
            if (!m_AnimationTimer) {
                m_AnimationTimer = [NSTimer scheduledTimerWithTimeInterval:1.0f target:self selector:@selector(timerFired) userInfo:nil repeats:YES];
            }
        }
        
        AYANodeView *nodeView = [[AYANodeView alloc] initWithFrame:CGRectMake(100, 100, 50, 50)];
        [nodeView setCenter:[sender locationInView:self.view]];
    
        UIPanGestureRecognizer* panGestureRecognizer = [[UIPanGestureRecognizer alloc] init];
        [panGestureRecognizer setMinimumNumberOfTouches:1];
        [panGestureRecognizer setMaximumNumberOfTouches:1];
        [panGestureRecognizer addTarget:self action:@selector(handlePan:)];
        [nodeView addGestureRecognizer:panGestureRecognizer];
            
        UITapGestureRecognizer* tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
        [tapGestureRecognizer setNumberOfTouchesRequired:1];
        [tapGestureRecognizer setNumberOfTapsRequired:2];
        [tapGestureRecognizer addTarget:self action:@selector(handleTap:)];
        [nodeView addGestureRecognizer:tapGestureRecognizer];
        
        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] init];
        [longPressGestureRecognizer setAllowableMovement:10.0f];
        [longPressGestureRecognizer setMinimumPressDuration:1.0f];
        [longPressGestureRecognizer setNumberOfTouchesRequired:1];
        [longPressGestureRecognizer addTarget:self action:@selector(handlePress:)];

        [nodeView addGestureRecognizer:longPressGestureRecognizer];
        
        [nodes addObject:nodeView];
        [self.view addSubview:nodeView];
    }else if(modeSelection.selectedSegmentIndex == 1){
        if (startView == nil && [sender.view isKindOfClass:[AYANodeView class]]) {
            startView = (AYANodeView*)[sender view];
            [startView.backgroundLayer setBackgroundColor:[UIColor colorWithRed:100.0/255.0 green:149.0/255.0 blue:155.0/255.0 alpha:1.0].CGColor];
        }else if([sender.view isKindOfClass:[AYANodeView class]] && !CGRectEqualToRect(sender.view.frame,startView.frame)){
            [CATransaction setDisableActions:YES];
            
            CALayer *lineLayer = [CALayer layer];
            lineLayer.opacity = 0.2;
            lineLayer.backgroundColor = [UIColor orangeColor].CGColor;
            
            [lineLayer setValue:@(kConnectionTypeLine) forKeyPath:@"connectionType"];
            
            AYAConnection *connection = [[AYAConnection alloc] init];
            [connection setLineLayer:lineLayer];
            [connection setStartView:startView];
            [connection setEndView:(AYANodeView*)[sender view]];
            [connection setProbability:(arc4random_uniform(500)/1000.0 + 0.5)];
            [[startView connectionArray] addObject:connection];

            [startView.layer addSublayer:lineLayer];
            
            CGPoint pos = startView.center;
            CGPoint target = [sender view].center;
            target.x -= pos.x;
            target.y -= pos.y;
            
            [self setLayerToLineFromAToB:lineLayer forA:CGPointZero andB:target andLineWidth:8];
            
            UIColor *colorOne = [UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0];
            UIColor *colorTwo = [UIColor colorWithRed:255.0/255.0 green:94.0/255.0 blue:58.0/255.0 alpha:1.0];
            
            CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"colors"];
            animation.duration = 5.0f;
            animation.delegate =self;
            animation.fromValue = colorOne;
            animation.toValue = colorTwo;
            [animation setAutoreverses:YES];
            [animation setRepeatCount:10000];
            [lineLayer addAnimation:animation forKey:@"animateColors"];
            
            [CATransaction setDisableActions:NO];
            
            [startView.backgroundLayer setBackgroundColor:[UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0].CGColor];
            startView = nil;
        }else if([sender.view isKindOfClass:[AYANodeView class]] && CGRectEqualToRect(sender.view.frame,startView.frame)){
            // Set up the shape of the circle
            int radius = 60;
            CAShapeLayer *circle = [CAShapeLayer layer];
            // Make a circular shape
            circle.path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(sender.view.bounds.size.width-radius, sender.view.bounds.size.height/2.0, 1.5*radius, 2.0*radius)
                                                     cornerRadius:radius].CGPath;
            // Center the shape in self.view
            circle.position = CGPointMake(0.0, 0.0);
            
            // Configure the apperence of the circle
            circle.opacity = 0.2;
            circle.fillColor = [UIColor clearColor].CGColor;
            circle.strokeColor = [UIColor orangeColor].CGColor;
            circle.lineWidth = 5;
            
            [circle setValue:@(kConnectionTypeCircle) forKeyPath:@"connectionType"];

            //add to connection array;
            AYAConnection *connection = [[AYAConnection alloc] init];
            [connection setLineLayer:circle];
            [connection setStartView:startView];
            [connection setEndView:(AYANodeView*)[sender view]];
            [connection setProbability:(arc4random_uniform(1000)/1000.0)];

            [[startView connectionArray] addObject:connection];
            // Add to parent layer
            [sender.view.layer addSublayer:circle];
            
            UIColor *colorOne = [UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0];
            UIColor *colorTwo = [UIColor colorWithRed:255.0/255.0 green:94.0/255.0 blue:58.0/255.0 alpha:1.0];
            
            CABasicAnimation *animation = [CABasicAnimation animationWithKeyPath:@"colors"];
            animation.duration = 5.0f;
            animation.delegate =self;
            animation.fromValue = colorOne;
            animation.toValue = colorTwo;
            [animation setAutoreverses:YES];
            [animation setRepeatCount:10000];
            [circle addAnimation:animation forKey:@"animateColors"];
            
            [startView.backgroundLayer setBackgroundColor:[UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0].CGColor];

            startView = nil;

        }
    }
    
}

-(void)setLayerToLineFromAToB:(CALayer *)layer forA:(CGPoint)a andB:(CGPoint)b andLineWidth:(CGFloat)lineWidth
{
    CGPoint center = { 0.5 * (a.x + b.x)+ [layer superlayer].bounds.size.width / 2.0, 0.5 * (a.y + b.y) + [layer superlayer].bounds.size.height / 2.0};
    CGFloat length = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    CGFloat angle = atan2(a.y - b.y, a.x - b.x);
    
    layer.position = center;
    layer.bounds = (CGRect) { {0, 0}, { length + lineWidth, lineWidth } };
    layer.transform = CATransform3DMakeRotation(angle, 0, 0, 1);
}

-(void)timerFired{
    [[nodes objectAtIndex:0] recievedEvent];
}

-(void)handlePan:(UIPanGestureRecognizer*)sender{
    if (modeSelection.selectedSegmentIndex == 0 && [sender.view isKindOfClass:[AYANodeView class]]) {
        CGPoint translation = [sender translationInView:self.view];
        sender.view.center = CGPointMake(sender.view.center.x + translation.x,sender.view.center.y + translation.y);
        [sender setTranslation:CGPointMake(0, 0) inView:self.view];
        [self updateLinesForView:(AYANodeView*)sender.view];
    }

}

-(void)handlePress:(UILongPressGestureRecognizer*)sender{
    if (sender.state == UIGestureRecognizerStateBegan) {
        NSLog(@"longPress");
        AYATableViewController *detailVC = [[AYATableViewController alloc] init];
        UINavigationController *navBar=[[UINavigationController alloc]initWithRootViewController:detailVC];
        [detailVC setModalPresentationStyle:UIModalPresentationFormSheet];
        [detailVC setPresentingNode:(AYANodeView*)sender.view];
        [navBar setModalPresentationStyle:UIModalPresentationFormSheet];
        [self.navigationController presentViewController:navBar animated:YES completion:nil];
    }
}

- (void)updateLinesForView:(AYANodeView *)draggableView
{
    for (AYANodeView *node in nodes) {
        for (AYAConnection *connection in node.connectionArray) {
            if ([[connection.lineLayer valueForKeyPath:@"connectionType"] intValue] == kConnectionTypeLine) {
                [CATransaction setDisableActions:YES];
                CALayer *lineLayer = connection.lineLayer;

                CGPoint pos = connection.startView.center;
                CGPoint target = connection.endView.center;
                target.x -= pos.x;
                target.y -= pos.y;
                [self setLayerToLineFromAToB:lineLayer forA:CGPointZero andB:target andLineWidth:8];
                [CATransaction setDisableActions:NO];
            }
        }
    }

}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}




@end
