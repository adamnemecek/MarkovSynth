//
//  TRPLeftDrawerViewController.m
//  Tripster
//
//  Created by Brian Gerstle on 3/23/14.
//  Copyright (c) 2014 UMiami. All rights reserved.
//

#import "TRPLeftDrawerViewController.h"



typedef NS_ENUM(NSInteger, TRPLeftDrawerCell) {
    TRPLeftDrawerCell_User = 0,
    TRPLeftDrawerCell_Logout,
    TRPLeftDrawerCellCount
};

#define kLeftDrawerCell @"LeftDrawerCell"

@interface TRPLeftDrawerViewController ()
@end

@implementation TRPLeftDrawerViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor darkGrayColor];
    [self.tableView registerClass:[UITableViewCell class] forCellReuseIdentifier:kLeftDrawerCell];
    [self.tableView reloadData];
    self.navigationController.navigationBarHidden = YES;
   }


-(void)viewWillAppear:(BOOL)animated{
    [self.tableView reloadData];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - UITableViewDataSource

- (UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:kLeftDrawerCell];
    cell.backgroundColor = [UIColor darkGrayColor];
    cell.textLabel.textColor = [UIColor whiteColor];
    NSString *cellText = [[NSString alloc] init];
    if (indexPath.section == 0) {
        switch (indexPath.row) {
            case 0:
                cellText = @"Clear Graph";
                break;
            case 1:
                cellText = @"Save";
                break;
            case 2:
                cellText = @"Load";
                break;
            case 3:
                cellText = @"Stop Notes";
                
            default:
                break;
        }
        [cell.textLabel setText:cellText];
    }else if (indexPath.section == 1) {
        switch (indexPath.row) {
            case 0:
                cellText = @"Synth Settings";
                break;
            case 1:
                cellText = @"Bach";
                break;
            case 2:
                cellText = @"Debussey";
                break;
        
            default:
                break;
        }
        [cell.textLabel setText:cellText];
    }
    return cell;
}

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 50.f;
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return (NSInteger)2;
}


- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    NSString *string = [[NSString alloc] init];
    if(section == 0)
        string = @"Settings";
    
    if(section == 1)
        string =  @"Other Stuff";

    
    return string;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return (NSInteger)5;
}

#pragma mark - UITableViewDelegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [tableView deselectRowAtIndexPath:indexPath animated:YES];

    if (indexPath.section ==0) {
        if (indexPath.row == 0) {
            [self.mainVC clearGraph];
        }
        if (indexPath.row == 1) {
            AYASaveStateViewController *detailVC = [[AYASaveStateViewController alloc] init];
            [detailVC setMainVC:self.mainVC];
            [detailVC setDelegate:self];
            [detailVC setIsSaving:true];
            UINavigationController *navBar=[[UINavigationController alloc]initWithRootViewController:detailVC];
            [navBar setModalPresentationStyle:UIModalPresentationFormSheet];
            [self.navigationController presentViewController:navBar animated:YES completion:nil];
        }
        if (indexPath.row == 2) {
            AYASaveStateViewController *detailVC = [[AYASaveStateViewController alloc] init];
            [detailVC setMainVC:self.mainVC];
            [detailVC setDelegate:self];
            [detailVC setIsSaving:false];
            UINavigationController *navBar=[[UINavigationController alloc]initWithRootViewController:detailVC];
            [navBar setModalPresentationStyle:UIModalPresentationFormSheet];
            [self.navigationController presentViewController:navBar animated:YES completion:nil];
        }
        if (indexPath.row == 3) {
            [self.mainVC stopNotes];
        }
    }else if (indexPath.section == 1){
        if (indexPath.row == 0) {
            AYASynthSettingsViewController *detailVC = [[AYASynthSettingsViewController alloc] init];
            [detailVC setMainVC:self.mainVC];
            UINavigationController *navBar=[[UINavigationController alloc]initWithRootViewController:detailVC];
            [navBar setModalPresentationStyle:UIModalPresentationFormSheet];
            [self.navigationController presentViewController:navBar animated:YES completion:nil];
        }
        if (indexPath.row == 1) {
            NSMutableDictionary *markovProbs = [AYAMidiToMarkovCalc calcProbabilityForFile:@"bwv772"];
            [self.mainVC createGraphFromDict:markovProbs];
        }
        if (indexPath.row == 2) {
            NSMutableDictionary *markovProbs = [AYAMidiToMarkovCalc calcProbabilityForFile:@"deb_clai_format0"];
            [self.mainVC createGraphFromDict:markovProbs];
        }
    }
}

-(void)saveGraphWithSlotNum:(int)slot{
    [self.mainVC saveGraphinSlot:slot];
}
-(void)loadGraphWithSlotNum:(int)slot{
    [self.mainVC loadGraphfromSlot:slot];
}



@end
