//
//  AYATableViewController.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/11/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYATableViewController.h"

@interface AYATableViewController ()

@end

@implementation AYATableViewController

@synthesize presentingNode;

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Back" style:UIBarButtonItemStylePlain target:self action:@selector(selfDismissed)];
    NSArray *noteNames = @[@"C", @"C\u266F / D\u266D",@"D",@"D\u266F / E\u266D",@"E",@"F",@"F\u266F G\u266D",@"G",@"G\u266F / A\u266D",@"A", @"A\u266F / B\u266D",@"B" ];
    // Create the manager
    //
    self.manager = [[RETableViewManager alloc] initWithTableView:self.tableView];
    
    // Add a section
    //
    RETableViewSection *section = [RETableViewSection sectionWithHeaderTitle:@"Properties"];
    [self.manager addSection:section];
    
    REPickerItem* pickerItem = [REPickerItem itemWithTitle:@"Note" value:@[@"C", @"4"] placeholder:nil options:@[noteNames, @[@"1", @"2", @"3", @"4", @"5", @"6",@"7", @"8"]]];
    pickerItem.onChange = ^(REPickerItem *item){
        int selectedName = 0;
        for (int i=0; i<[noteNames count]; i++) {
            if ([[item.value objectAtIndex:0] isEqualToString:[noteNames objectAtIndex:i]]) {
                selectedName = i;
            }
        }
        int notenumber = selectedName + ([[item.value objectAtIndex:1] intValue]-1) * 12 + 24;

        NSLog(@"Value: %@, %d", item.value, notenumber);
        
        [presentingNode setNote:notenumber];
        NSString *noteName = [NSString stringWithFormat:@"%@ %@",[item.value objectAtIndex:0],[item.value objectAtIndex:1]];
        [presentingNode setNoteName:noteName];
        
        
    };
    
    // Use inline picker in iOS 7
    //
    if (REUIKitIsFlatMode()) {
        pickerItem.inlinePicker = YES;
    }
    
    [section addItem:pickerItem];
    
    // Add a float item
    //
//    [section addItem:[REFloatItem itemWithTitle:@"Float item" value:connection sliderValueChangeHandler:^(REFloatItem *item) {
//        NSLog(@"Value: %f", item.value);
//    }]];
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
    
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}

-(void)selfDismissed{
    [self dismissViewControllerAnimated:YES completion:nil];
}
-(void)viewWillAppear:(BOOL)animated{
    RETableViewSection *section = [RETableViewSection sectionWithHeaderTitle:@"Connections"];
    [self.manager addSection:section];
    for (AYAConnection *connection in presentingNode.connectionArray) {
        NSString *title = [NSString stringWithFormat:@"To :%@",connection.endView.noteName];
        [((RETableViewSection*)[self.manager.sections objectAtIndex:1]) addItem:[REFloatItem itemWithTitle:title value:connection.probability sliderValueChangeHandler:^(REFloatItem *item) {
//            NSLog(@"Value: %f, %d", item.value, item.indexPath.row);
            [[presentingNode.connectionArray objectAtIndex:item.indexPath.row] setProbability:item.value];
            }]];
    }
    int lengthind = 0;
    if (presentingNode.noteLength == 1.0f) {
        lengthind = 0;
    }else if(presentingNode.noteLength == 0.5f){
        lengthind = 1;
    }else if(presentingNode.noteLength == 0.25f){
        lengthind = 2;
    }else if(presentingNode.noteLength == 0.125f){
        lengthind = 3;
    }
    [((RETableViewSection*)self.manager.sections[0]) addItem:[RESegmentedItem itemWithTitle:@"Length" segmentedControlTitles:@[@"Half",@"Quarter", @"8th", @"16th"] value:lengthind switchValueChangeHandler:^(RESegmentedItem *item) {
        NSLog(@"Value: %li", (long)item.value);
        switch ((long)item.value) {
            case 0:
                presentingNode.noteLength = 1.0f;
                break;
            case 1:
                presentingNode.noteLength = 0.5f;
                break;
            case 2:
                presentingNode.noteLength = 0.25f;
                break;
            case 3:
                presentingNode.noteLength = 0.125f;
                break;
            default:
                break;
        }
    }]];

    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    // Return the number of sections.
    return 0;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return 0;
}

/*
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:<#@"reuseIdentifier"#> forIndexPath:indexPath];
    
    // Configure the cell...
    
    return cell;
}
*/

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
