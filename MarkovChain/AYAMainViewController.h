//
//  AYAMainViewController.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/10/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AYAConnection.h"
#import "AYAConnectionManager.h"
#import "AYANodeView.h"
#import "AYATableViewController.h"

@interface AYAMainViewController : UIViewController{
    
    AYAConnectionManager *connectionManager;
    
    __weak IBOutlet UISegmentedControl *modeSelection;
    
    AYANodeView* startView;
    
    NSMutableArray *nodes;
    
    NSTimer *m_AnimationTimer;
}

@property (weak, nonatomic) IBOutlet UITableView *tableView;


@end
