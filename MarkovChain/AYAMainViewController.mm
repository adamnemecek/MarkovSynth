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

@synthesize auEngine;

- (void)splitViewController:(UISplitViewController *)svc willHideViewController:(UIViewController *)aViewController withBarButtonItem:(UIBarButtonItem *)barButtonItem forPopoverController:(UIPopoverController *)pc{
    self.navigationItem.leftBarButtonItem = barButtonItem;
}

-(void)splitViewController:(UISplitViewController *)svc willShowViewController:(UIViewController *)aViewController invalidatingBarButtonItem:(UIBarButtonItem *)barButtonItem{
    self.navigationItem.leftBarButtonItem = barButtonItem;
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
        // create frequency table for MIDI support
        // make frequency (Hz) table
        currentOctave = 4;
        double k = 1.059463094359;	// 12th root of 2
        double a = 6.875;	// a
        a *= k;	// b
        a *= k;	// bb
        a *= k;	// c, frequency of midi note 0
        for (int i = 0; i < 127; i++)	// 128 midi notes
        {
            // Hz Table
            m_MIDIFreqTable[i] = (float)a;
            
            // update for loop . . .
            a *= k;
        }
        m_dDetune_cents = 10.0;
        m_uOsc1Waveform = 4;
        m_uOsc2Waveform = 1;
        m_fOsc1Level = 33.0;
        m_fOsc2Level = 33.0;
        m_uDetuneSemitones = 0;
        m_nOctave = 0;
        m_dFcControl = 800.0;
        m_dRate_LFO = 2.0;
        m_dAttackTime_mSec = 50.0;
        m_dPulseWidth_Pct = 20;
        m_dPortamentoTime_mSec = 0;
        m_dQControl = 5.0;
        m_dOscLFOIntensity = 0.0;
        m_dDecayReleaseTime_mSec = 100.0;
        m_uModMode = 0;
        
        m_dHSRatio = 1.00;
        m_dOscEGIntensity = 0.0;
        m_dFilterEGIntensity = 0.0;
        m_dFilterLFOIntensity = 0.0;
        
        m_dSustainLevel = 0.5;
        m_dLFOAmpIntensity = 0.0;
        m_dLFOPanIntensity = 0.0;
        m_uLFO_Waveform = 0;
        m_dDCAEGIntensity = 1.00;
        
        //Overall Synth Stuff
        m_uLegatoMode = 0;
        m_nPitchBendRange = 1;
        m_uResetToZero = 0;
        m_uFilterKeyTrack = 0;
        m_dFilterKeyTrackIntensity = 1.0;
        m_uVelocityToAttackScaling = 0;
        m_uNoteNumberToDecayScaling = 0;
        
        //Timbre
//        m_uTimbreSelection = 0;
        
    }
    return self;
}
-(void)updateMiniSynth
{
    CMiniSynthVoice* pVoice;
    for(int i=0; i<16; i++)
    {
        timbre *pTimbre = &timbre1;
        pVoice = auEngine->m_VoicePtrStack1[i];
        
        pTimbre->Detune_cents = m_dDetune_cents;
        pVoice->setDetuneCents(pTimbre->Detune_cents);
        
        pTimbre->Octave = m_nOctave;
        pVoice->setOctave(pTimbre->Octave);
        
        pTimbre->Osc1Waveform = m_uOsc1Waveform;
        pVoice->setOsc1Waveform(pTimbre->Osc1Waveform);
        
        pTimbre->Osc2Waveform = m_uOsc2Waveform;
        pVoice->setOsc2Waveform(pTimbre->Osc2Waveform);
        
        pTimbre->Osc1Level = m_fOsc1Level;
        pVoice->setOsc1Level(pTimbre->Osc1Level);
        
        pTimbre->Osc2Level = m_fOsc2Level;
        pVoice->setOsc2Level(pTimbre->Osc2Level);
 
        pTimbre->DetuneSemitones = m_uDetuneSemitones;
        pVoice->setOscDetuneSemitones(m_uDetuneSemitones);
        
        pTimbre->ModMode = m_uModMode;
        pVoice->setModMode(pTimbre->ModMode);
        
        pTimbre->LegatoMode = (bool)m_uLegatoMode;
        pVoice->setEG1LegatoMode(pTimbre->LegatoMode);
        
        pTimbre->ResetToZero = (bool)m_uResetToZero;
        pVoice->setEG1ResetToZero(pTimbre->ResetToZero);
        
        pTimbre->PitchBendRange = m_nPitchBendRange;
        pVoice->setPitchBendModRange(pTimbre->PitchBendRange);
        // PORTAMENTO!
        
        pTimbre->PortamentoTime_mSec = m_dPortamentoTime_mSec;
        pVoice->setPortamentoTime_mSec(pTimbre->PortamentoTime_mSec);
        
        pTimbre->HSRatio = m_dHSRatio;
        pVoice->setHSRatio(pTimbre->HSRatio);
        
        pTimbre->OscEGIntensity = m_dOscEGIntensity;
        pVoice->setOscEGIntensity(pTimbre->OscEGIntensity);

        // col 2
        pTimbre->FcControl = m_dFcControl;
        pVoice->setFilter1Cutoff(pTimbre->FcControl);
        
        pTimbre->QControl = m_dQControl;
        pVoice->setFilter1Q(pTimbre->QControl);
        
        pTimbre->FilterEGIntensity = m_dFilterEGIntensity;
        pVoice->setFilter1EGIntensity(pTimbre->FilterEGIntensity);
        
        pTimbre->LFOAmpIntensity = m_dLFOAmpIntensity;
        pVoice->setLFO1DCAAmpModIntensity(pTimbre->LFOAmpIntensity);
        // col 3
        pTimbre->Rate_LFO = m_dRate_LFO;
        pVoice->setLFO1Rate(pTimbre->Rate_LFO);
        
        pTimbre->OscLFOIntensity = m_dOscLFOIntensity;
        pVoice->setOscLFOIntensity(pTimbre->OscLFOIntensity);
        
        pTimbre->FilterLFOIntensity = m_dFilterLFOIntensity;
        pVoice->setFilter1LFOIntensity(pTimbre->FilterLFOIntensity);
        
        pTimbre->LFOWaveform = m_uLFO_Waveform;
        pVoice->setLFO1Waveform(pTimbre->LFOWaveform);
        
        pTimbre->LFOPanIntensity = m_dLFOPanIntensity;
        pVoice->setDCAPanLFOIntensity(pTimbre->LFOPanIntensity);
        // col 4
        
        pTimbre->AttackTime_mSec = m_dAttackTime_mSec;
        pVoice->setEG1AttackTime_mSec(m_dAttackTime_mSec);
        
        pTimbre->DecayReleaseTime_mSec = m_dDecayReleaseTime_mSec;
        pVoice->setEG1DecayTime_mSec(m_dDecayReleaseTime_mSec);
        pVoice->setEG1ReleaseTime_mSec(m_dDecayReleaseTime_mSec);
        
        pTimbre->SustainLevel = m_dSustainLevel;
        pVoice->setEG1SustainLevel(m_dSustainLevel);
        
        pTimbre->DCAEGIntensity = m_dDCAEGIntensity;
        pVoice->setDCAEGIntensity(m_dDCAEGIntensity);
        // turn on/off and Intensity; filter key track
        
        pTimbre->PulseWidth_Pct = m_dPulseWidth_Pct;
        pVoice->setPulseWidthControl(m_dPulseWidth_Pct);
        
        pVoice->setLFO1DCAAmpModIntensity(m_dLFOAmpIntensity);
        
        pVoice->setGain(1.0);
        
        pVoice->setModMode(0);
        pVoice->update();
    }
}
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.navigationController.navigationBarHidden = YES;
    
    path = [UIBezierPath bezierPath];
    tempHitArray = [[NSMutableArray alloc] init];
    drawView = [[AYADrawView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:drawView];
    // Initialize the engine to render our synthesizer
    auEngine = [[AudioUnitsEngine alloc] init];
    // Call the initial update to set the parameters to the values defined in this class
    [self updateMiniSynth];
    // We always call the stop before initializing it just to make sure
    [auEngine stopAUGraph];
    // Start the rendering
	[auEngine startAUGraph];
    
    // Create our connection Manager
    connectionManager = [[AYAConnectionManager alloc] init];
    
    // Initialize the nodes array
    nodes = [[NSMutableArray alloc] init];
    
    //Create the background layer for the main view.
    CAGradientLayer *backgroundLayer = [[CAGradientLayer alloc] init];
    backgroundLayer.frame = self.view.bounds;
    
    // Set up our two colors for the nice gradient
    UIColor *colorOne = [UIColor colorWithRed:74.0/255.0 green:74.0/255.0 blue:74.0/255.0 alpha:1.0];
    UIColor *colorTwo = [UIColor colorWithRed:43.0/255.0 green:43.0/255.0 blue:43.0/255.0 alpha:1.0];
    NSArray *colors =  @[(id)colorOne.CGColor, (id)colorTwo.CGColor]; //<- Literals!
    
    // Setup color stop locations
    NSNumber *stopOne = [NSNumber numberWithFloat:0.1];
    NSNumber *stopTwo = [NSNumber numberWithFloat:0.8];
    NSArray *locations = @[stopOne, stopTwo]; //<- Literals!
    
    // Finish Background Layer stuff
    backgroundLayer.colors = colors;
    backgroundLayer.locations = locations;
    [self.view.layer addSublayer:backgroundLayer];
    
    // Make sure that the mode selection view is always in front of the background layer
    
    [self.view bringSubviewToFront:drawView];
    
    [self.view bringSubviewToFront:modeSelection];
    
    
    RLStereoDelay *stereoDelay = [[RLStereoDelay alloc] init];
    [auEngine addEffectorEffectGroupToArray:stereoDelay forKey:@"stereoDelay"];
    [stereoDelay setParameterValue:@(500) forKey:@"delay_ms"];
    [stereoDelay setParameterValue:@(0.5) forKey:@"feedback_norm"];
    [stereoDelay setParameterValue:@(0.3) forKey:@"wetMix_norm"];

    
}

-(void)viewDidAppear:(BOOL)animated{
    // Double tap recognizer to add node.
    UITapGestureRecognizer* tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
    [tapGestureRecognizer setNumberOfTouchesRequired:1];
    [tapGestureRecognizer setNumberOfTapsRequired:2];
    [tapGestureRecognizer addTarget:self action:@selector(handleTap:)];
    [self.view addGestureRecognizer:tapGestureRecognizer];
    
    UIPanGestureRecognizer* panGestureRecognizer = [[UIPanGestureRecognizer alloc] init];
    [panGestureRecognizer setMinimumNumberOfTouches:1];
    [panGestureRecognizer setMaximumNumberOfTouches:1];
    [panGestureRecognizer addTarget:self action:@selector(handleDrawPan:)];
    [self.view addGestureRecognizer:panGestureRecognizer];
}

-(void)handleTap:(UITapGestureRecognizer*)sender{
    
        // Init our node view and assign it to the place that was tapped.
        CGPoint currentPoint = [sender locationInView:self.view];
    
    
        // Figure out what was hit
        AYANodeView *tappedNode;
        BOOL nodehit = false;
        AYAConnection *tappedConnection;
        BOOL conhit = false;
        for (AYANodeView *node in nodes){
            nodehit = CGRectContainsPoint(node.frame, currentPoint);
            if (nodehit) {
                tappedNode = node;
                break;
            }
        }
    
        if (!nodehit) {
            for (AYANodeView *node in nodes){
                for(AYAConnection *connection in [node.connectionArray copy])
                {
                    CGPoint currentPoint = [sender locationInView:node];
                    
                    conhit = [connection.lineLayer.presentationLayer containsPoint:currentPoint];
                    if(conhit)
                    {
                        tappedConnection = connection;
                        break;
                    }
                }
            }
        }
    
    
        if (nodehit) {
            [nodes removeObject:tappedNode];
            [tappedNode.layer removeAllAnimations];
            for (CALayer *layer in tappedNode.layer.sublayers) {
                [layer removeAllAnimations];
            }
            
            [tappedNode removeFromSuperview];
            
            for (AYANodeView *node in nodes){
                for(AYAConnection *connection in [node.connectionArray copy])
                {
                    if (connection.endView == tappedNode) {
                        [connection.lineLayer removeFromSuperlayer];
                        [node.connectionArray removeObject:connection];
                    }
                }
            }
        }
        else if(conhit)
        {
            [tappedConnection.lineLayer removeFromSuperlayer];
            [tappedConnection.startView.connectionArray removeObject:tappedConnection];
        }
        else
        {
            AYANodeView *nodeView = [[AYANodeView alloc] initWithFrame:CGRectMake(0, 0, 76, 76)];
            [nodeView setCenter:[sender locationInView:self.view]];
            // We need to be the node's delegate as well.
            [nodeView setDelegate:self];
        
            // Pan gesture to move nodes
            UIPanGestureRecognizer* panGestureRecognizer = [[UIPanGestureRecognizer alloc] init];
            [panGestureRecognizer setMinimumNumberOfTouches:1];
            [panGestureRecognizer setMaximumNumberOfTouches:1];
            [panGestureRecognizer addTarget:self action:@selector(handlePan:)];
            [nodeView addGestureRecognizer:panGestureRecognizer];
            
            
            // Double tap for connection creation, this is only until I get drawing connections done.
            UITapGestureRecognizer* tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
            [tapGestureRecognizer setNumberOfTouchesRequired:1];
            [tapGestureRecognizer setNumberOfTapsRequired:2];
            [tapGestureRecognizer addTarget:self action:@selector(handleTap:)];
            [nodeView addGestureRecognizer:tapGestureRecognizer];
            
            // Long press to bring up form sheet to edit properties. Rob had a good idea of using a pinch or drag from a node to present the controls that this shows.
            UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] init];
            [longPressGestureRecognizer setAllowableMovement:10.0f];
            [longPressGestureRecognizer setMinimumPressDuration:1.0f];
            [longPressGestureRecognizer setNumberOfTouchesRequired:1];
            [longPressGestureRecognizer addTarget:self action:@selector(handlePress:)];
            [nodeView addGestureRecognizer:longPressGestureRecognizer];
            
            // Add the nodeView to the array we use to keep track of all the nodes.
            [nodes addObject:nodeView];
            
            // Finally, after much ado, we add the view to the mainVC
            [self.view addSubview:nodeView];
        }

}

-(void)setLayerToLineFromAToB:(CALayer *)layer forA:(CGPoint)a andB:(CGPoint)b andLineWidth:(CGFloat)lineWidth
{
    CGPoint center = { static_cast<CGFloat>(0.5 * (a.x + b.x)+ [layer superlayer].bounds.size.width / 2.0), static_cast<CGFloat>(0.5 * (a.y + b.y) + [layer superlayer].bounds.size.height / 2.0)};
    CGFloat length = sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
    CGFloat angle = atan2(a.y - b.y, a.x - b.x);
    
    layer.position = center;
    layer.bounds = (CGRect) { {0, 0}, { length + lineWidth, lineWidth } };
    layer.transform = CATransform3DMakeRotation(angle, 0, 0, 1);
}

-(void)handlePan:(UIPanGestureRecognizer*)sender{
    if (modeSelection.selectedSegmentIndex == 0) {
        CGPoint translation = [sender translationInView:self.view];
        sender.view.center = CGPointMake(sender.view.center.x + translation.x,sender.view.center.y + translation.y);
        [sender setTranslation:CGPointMake(0, 0) inView:self.view];
        [self updateLinesForView:(AYANodeView*)sender.view];
    }
}

-(void)handleDrawPan:(UIPanGestureRecognizer *)sender{
    if(modeSelection.selectedSegmentIndex ==1){
        CGPoint currentPoint = [sender locationInView:self.view];
        [drawView handleDrawPan:sender];
        for (AYANodeView *node in nodes){
            BOOL prevhit = CGRectContainsPoint(node.frame, previousPoint);
            BOOL hit = CGRectContainsPoint(node.frame, currentPoint);
            if (hit && !prevhit) {
                [tempHitArray addObject:node];
            }
            
        }
        if (sender.state == UIGestureRecognizerStateEnded) {
            [self handlehits];
        }
        previousPoint = currentPoint;
    }
}

-(void)handlehits{
    if ([tempHitArray count]>0) {
        for (int i=0; i<[tempHitArray count]-1; i++) {
            [self makeConnectionFromView:tempHitArray[i] toView:tempHitArray[i+1]];
        }
    }
    [tempHitArray removeAllObjects];
}

-(void)makeConnectionFromView:(AYANodeView*)startView toView:(AYANodeView*)endView
{
    [self makeConnectionFromView:startView toView:endView withProbability:(arc4random_uniform(500)/1000.0 + 0.5)];
}
-(void)makeConnectionFromView:(AYANodeView*)startView toView:(AYANodeView*)endView withProbability:(float)prob
{
    // Or, we already have a startview and we need to create the connection (not to ourselves).
    if([endView isKindOfClass:[AYANodeView class]] && !CGRectEqualToRect(endView.frame,startView.frame)){
        [CATransaction setDisableActions:YES];
        
        // Create a line layer
        CALayer *lineLayer = [CALayer layer];
        lineLayer.opacity = 0.2;
        lineLayer.backgroundColor = [UIColor orangeColor].CGColor;
        
        // It will be a line connection
        [lineLayer setValue:@(kConnectionTypeLine) forKeyPath:@"connectionType"];
        
        // This is the data model of a connection that lives behind the visualization conceptually
        AYAConnection *connection = [[AYAConnection alloc] init];
        [connection setLineLayer:lineLayer];
        [connection setStartView:startView];
        [connection setEndView:(AYANodeView*)endView];
        [connection setProbability:prob];
        [[startView connectionArray] addObject:connection];
        
        // Add our layer
        [startView.layer insertSublayer:lineLayer below:startView.backgroundLayer];
        // Get it set up to be positioned and referenced
        CGPoint pos = startView.center;
        CGPoint target = endView.center;
        target.x -= pos.x;
        target.y -= pos.y;
        // Put the layer in the right place.
        [self setLayerToLineFromAToB:lineLayer forA:CGPointZero andB:target andLineWidth:8];
        
        // Add the nice gradient animation to the lines as well.
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
        
        // Remove the special coloring on the startview's background
        [startView.backgroundLayer setBackgroundColor:[UIColor colorWithRed:255.0/255.0 green:149.0/255.0 blue:0.0/255.0 alpha:1.0].CGColor];
        
        // reset startview to nil
        startView = nil;
        
        // We're making a connection, but to self.
    }else if([endView isKindOfClass:[AYANodeView class]] && CGRectEqualToRect(endView.frame,startView.frame)){
        // Set up the shape of the circle
        int radius = 60;
        CAShapeLayer *circle = [CAShapeLayer layer];
        // Make a circular shape
        circle.path = [UIBezierPath bezierPathWithRoundedRect:CGRectMake(endView.bounds.size.width-radius, endView.bounds.size.height/2.0, 1.5*radius, 2.0*radius)
                                                 cornerRadius:radius].CGPath;
        // Center the shape in self.view
        circle.position = CGPointMake(0.0, 0.0);
        
        // Configure the apperence of the circle
        circle.opacity = 0.2;
        circle.fillColor = [UIColor clearColor].CGColor;
        circle.strokeColor = [UIColor orangeColor].CGColor;
        circle.lineWidth = 5;
        
        // set the connection type.
        [circle setValue:@(kConnectionTypeCircle) forKeyPath:@"connectionType"];
        
        //add to connection array;
        AYAConnection *connection = [[AYAConnection alloc] init];
        [connection setLineLayer:circle];
        [connection setStartView:startView];
        [connection setEndView:endView];
        [connection setProbability:(arc4random_uniform(1000)/1000.0)];
        
        [[startView connectionArray] addObject:connection];
        // Add to parent layer
        [endView.layer insertSublayer:circle below:startView.backgroundLayer];
        
        //Gradient stuff
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

-(CGPoint) midpointFromPoint:(CGPoint)p0 andPoint:(CGPoint) p1
{
    CGPoint point = CGPointMake((p0.x + p1.x) / 2.0, (p0.y + p1.y) / 2.0);
    return point;
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
-(BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc inOrientation:(UIInterfaceOrientation)orientation
{
    return YES;
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

-(void)noteOn:(int)notenumber{
//    NSLog(@"Note ON: %d",notenumber);
    [auEngine setNoteOn:notenumber];
}

-(void)noteOff:(int)notenumber{
//    NSLog(@"Note OFF: %d",notenumber);
    [auEngine setNoteOff:notenumber];
}

-(void)stopNotes{
    for (AYANodeView *node in nodes) {
        [node.layer removeAllAnimations];
        for (CALayer *layer in node.layer.sublayers) {
            [layer removeAllAnimations];
        }
    }
}

-(void)clearGraph{
    for (AYANodeView *node in nodes) {
        [node.layer removeAllAnimations];
        for (CALayer *layer in node.layer.sublayers) {
            [layer removeAllAnimations];
        }
        
        [node removeFromSuperview];
        
    }
    
    [nodes removeAllObjects];
    
    [self.auEngine stopAUGraph];
    [self.auEngine startAUGraph];
}

-(void)saveGraphinSlot:(int)slot{

    NSString *string = [NSString stringWithFormat:@"%d.archive",slot];
    NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *documentDirectory = [[documentDirectories objectAtIndex:0] stringByAppendingPathComponent:string];
    NSMutableArray *nodeStore = [[NSMutableArray alloc] init];
    for (AYANodeView *node in nodes) {
        NSMutableDictionary *nodeDict = [[NSMutableDictionary alloc] init];
        [nodeDict setObject:[NSValue valueWithCGRect:node.frame ] forKey:@"frame"];
        [nodeDict setObject:@(node.note) forKey:@"note"];
        [nodeDict setObject:@(node.noteLength) forKey:@"length"];
        [nodeDict setObject:node.noteName forKey:@"noteName"];
        NSMutableArray *connectionArray = [[NSMutableArray alloc] init];
        for (AYAConnection *connection in node.connectionArray) {
            NSMutableDictionary *connectionDict = [[NSMutableDictionary alloc] init];
            [connectionDict setObject:[NSValue valueWithCGRect:connection.startView.frame] forKey:@"startFrame"];
            [connectionDict setObject:[NSValue valueWithCGRect:connection.endView.frame] forKey:@"endFrame"];
            [connectionDict setObject:@(connection.probability) forKey:@"prob"];
            [connectionArray addObject:connectionDict];
        }
        [nodeDict setObject:connectionArray forKey:@"connectionArray"];
        [nodeStore addObject:nodeDict];
    }
    [NSKeyedArchiver archiveRootObject:nodeStore toFile:documentDirectory];
    
}

-(void)loadGraphfromSlot:(int)slot{
    for (AYANodeView *node in nodes) {
        [node removeFromSuperview];
    }
    
    [nodes removeAllObjects];
    
    NSString *string = [NSString stringWithFormat:@"%d.archive",slot];
    NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *documentDirectory = [[documentDirectories objectAtIndex:0] stringByAppendingPathComponent:string];
    NSArray *nodeStore = [NSKeyedUnarchiver unarchiveObjectWithFile:documentDirectory];
    
    for(NSDictionary *nodeDict in nodeStore){
        AYANodeView *nodeView = [[AYANodeView alloc] initWithFrame:[nodeDict[@"frame"] CGRectValue]];
        // We need to be the node's delegate as well.
        [nodeView setDelegate:self];
        // Pan gesture to move nodes
        UIPanGestureRecognizer* panGestureRecognizer = [[UIPanGestureRecognizer alloc] init];
        [panGestureRecognizer setMinimumNumberOfTouches:1];
        [panGestureRecognizer setMaximumNumberOfTouches:1];
        [panGestureRecognizer addTarget:self action:@selector(handlePan:)];
        [nodeView addGestureRecognizer:panGestureRecognizer];
        
        // Double tap for connection creation, this is only until I get drawing connections done.
        UITapGestureRecognizer* tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
        [tapGestureRecognizer setNumberOfTouchesRequired:1];
        [tapGestureRecognizer setNumberOfTapsRequired:2];
        [tapGestureRecognizer addTarget:self action:@selector(handleTap:)];
        [nodeView addGestureRecognizer:tapGestureRecognizer];
        
        // Long press to bring up form sheet to edit properties. Rob had a good idea of using a pinch or drag from a node to present the controls that this shows.
        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] init];
        [longPressGestureRecognizer setAllowableMovement:10.0f];
        [longPressGestureRecognizer setMinimumPressDuration:1.0f];
        [longPressGestureRecognizer setNumberOfTouchesRequired:1];
        [longPressGestureRecognizer addTarget:self action:@selector(handlePress:)];
        [nodeView addGestureRecognizer:longPressGestureRecognizer];
        
        // Add the nodeView to the array we use to keep track of all the nodes.
        [nodes addObject:nodeView];
        [nodeView setNote:[nodeDict[@"note"] intValue]];
        [nodeView setNoteLength:[nodeDict[@"length"] floatValue]];
        [nodeView setNoteName:nodeDict[@"noteName"]];
        
        // Finally, after much ado, we add the view to the mainVC
        [self.view addSubview:nodeView];
    }
    
    for (NSDictionary *nodeDict in nodeStore) {
        for (NSDictionary *connectionDict in nodeDict[@"connectionArray"]) {
            AYANodeView *startNode = nil;
            AYANodeView *endNode = nil;
            for(AYANodeView *node in nodes){
                if (CGRectEqualToRect(node.frame, [connectionDict[@"startFrame"] CGRectValue])) {
                    startNode = node;
                }
                if (CGRectEqualToRect(node.frame, [connectionDict[@"endFrame"] CGRectValue])) {
                    endNode = node;
                }
            }
            [self makeConnectionFromView:startNode toView:endNode withProbability:[connectionDict[@"prob"] floatValue]];
            
        }
    }
}
-(NSString *)getStringForNoteNum:(int)noteNum{
    NSArray *noteNames = @[@"C", @"C\u266F / D\u266D",@"D",@"D\u266F / E\u266D",@"E",@"F",@"F\u266F G\u266D",@"G",@"G\u266F / A\u266D",@"A", @"A\u266F / B\u266D",@"B" ];
    NSArray *octaveString = @[@"-1",@"0",@"1", @"2", @"3", @"4", @"5", @"6",@"7", @"8",@"9",@"10"];
    
    NSString *noteName = noteNames[noteNum%12];
    NSString *noteOctave = [octaveString objectAtIndex:(floor(noteNum/12))];
    
    return [NSString stringWithFormat:@"%@ %@",noteName, noteOctave];
}


-(void)createGraphFromDict:(NSDictionary*)markovDicts{
    NSMutableArray *nodeViewArray = [[NSMutableArray alloc] init];
    for (int i=0; i<12; i++) {
        [nodeViewArray addObject:@(0)];
    }
    for(int i=0; i<[markovDicts count]; i++){
        AYANodeView *nodeView = [[AYANodeView alloc] initWithFrame:CGRectMake(i*30,200, 76, 76)];
        // We need to be the node's delegate as well.
        [nodeView setDelegate:self];
        // Pan gesture to move nodes
        UIPanGestureRecognizer* panGestureRecognizer = [[UIPanGestureRecognizer alloc] init];
        [panGestureRecognizer setMinimumNumberOfTouches:1];
        [panGestureRecognizer setMaximumNumberOfTouches:1];
        [panGestureRecognizer addTarget:self action:@selector(handlePan:)];
        [nodeView addGestureRecognizer:panGestureRecognizer];
        
        // Double tap for connection creation, this is only until I get drawing connections done.
        UITapGestureRecognizer* tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
        [tapGestureRecognizer setNumberOfTouchesRequired:1];
        [tapGestureRecognizer setNumberOfTapsRequired:2];
        [tapGestureRecognizer addTarget:self action:@selector(handleTap:)];
        [nodeView addGestureRecognizer:tapGestureRecognizer];
        
        // Long press to bring up form sheet to edit properties. Rob had a good idea of using a pinch or drag from a node to present the controls that this shows.
        UILongPressGestureRecognizer* longPressGestureRecognizer = [[UILongPressGestureRecognizer alloc] init];
        [longPressGestureRecognizer setAllowableMovement:10.0f];
        [longPressGestureRecognizer setMinimumPressDuration:1.0f];
        [longPressGestureRecognizer setNumberOfTouchesRequired:1];
        [longPressGestureRecognizer addTarget:self action:@selector(handlePress:)];
        [nodeView addGestureRecognizer:longPressGestureRecognizer];
        
        // Add the nodeView to the array we use to keep track of all the nodes.
        [nodes addObject:nodeView];
        [nodeView setNoteLength:-1.0];
        [nodeView setNote:[[markovDicts allKeys][i] intValue]+ 12*5];
        [nodeView setNoteName:[self getStringForNoteNum:nodeView.note]];

        [nodeViewArray setObject:nodeView atIndexedSubscript:(nodeView.note%12)];
        // Finally, after much ado, we add the view to the mainVC
        [self.view addSubview:nodeView];
        
    }
    
    for (AYANodeView *node in nodes) {
        NSDictionary *dict = [markovDicts objectForKey:@(node.note%12)];
        AYANodeView *startNode = node;
        for (AYANodeView *node in nodes) {
            if ([dict objectForKey:@(node.note%12)]) {
                [self makeConnectionFromView:startNode toView:node withProbability:[[dict objectForKey:@(node.note%12)] floatValue]];
            }
        }
    }
}

@end
