//
//  AYATableViewController.h
//  MarkovChain
//
//  Created by Andrew Ayers on 4/11/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <RETableViewManager/RETableViewManager.h>
#import "AYANodeView.h"
#import "AYAConnection.h"

@interface AYATableViewController : UITableViewController

@property RETableViewManager *manager;

@property AYANodeView *presentingNode;
@end
