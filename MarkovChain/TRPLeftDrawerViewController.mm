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

    if (indexPath.section == 0) {
        if (indexPath.row == 0) {
            [cell.textLabel setText:@"Clear"];
        }
        if (indexPath.row == 1) {
            [cell.textLabel setText:@"Save"];
        }
        if (indexPath.row == 2) {
            [cell.textLabel setText:@"Load"];
        }
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
        string =  @"Something Else";

    
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
            [self.mainVC saveGraph];
        }
        if (indexPath.row == 2) {
            [self.mainVC loadGraph];
        }
    }
}

@end