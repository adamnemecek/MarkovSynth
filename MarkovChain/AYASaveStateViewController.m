//
//  AYASaveStateViewController.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/24/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYASaveStateViewController.h"

@interface AYASaveStateViewController ()

@end


NSString *kCellID = @"cellID";                          // UICollectionViewCell storyboard id

@implementation AYASaveStateViewController

@synthesize delegate,mainVC,isSaving;

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
    // Do any additional setup after loading the view from its nib.
    
    self.view.backgroundColor = [UIColor clearColor];
    self.collectionView.backgroundColor = [UIColor clearColor];
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithTitle:@"Back" style:UIBarButtonItemStylePlain target:self action:@selector(selfDismissed)];
    [self.collectionView registerClass:[TRPGenericCollectionViewCell class] forCellWithReuseIdentifier:kCellID];
}

-(void)viewDidAppear:(BOOL)animated{
    [self.collectionView reloadData];
}

-(void)selfDismissed{
    [self dismissViewControllerAnimated:YES completion:nil];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section{
    return 20;
}


- (UICollectionViewCell *)collectionView:(UICollectionView *)cv cellForItemAtIndexPath:(NSIndexPath *)indexPath;
{
    // we're going to use a custom UICollectionViewCell, which will hold an image and its label
    //
    TRPGenericCollectionViewCell *cell = [cv dequeueReusableCellWithReuseIdentifier:kCellID forIndexPath:indexPath];
    
    // make the cell's title the actual NSIndexPath value
    [cell setTitle:[NSString stringWithFormat:@"%ld", (long)indexPath.row]];
    NSString *string = [NSString stringWithFormat:@"%ld.jpg",(long)indexPath.row];
    NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *documentDirectory = [[documentDirectories objectAtIndex:0] stringByAppendingPathComponent:string];
    UIImage *image = [UIImage imageWithContentsOfFile:documentDirectory];
    if (image) {
        [cell setBackgroundImage:image];
    }

    return cell;
}

-(void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath{
    if (isSaving) {
        NSLog(@"%ld",(long)indexPath.row);
        UIImage *tempImage = [self snapshot:((UIViewController*)mainVC).view];
        TRPGenericCollectionViewCell *cell = (TRPGenericCollectionViewCell*)[collectionView cellForItemAtIndexPath:indexPath];
        tempImage = [AYASaveStateViewController imageWithImage:tempImage scaledToSize:cell.frame.size];
        // Convert UIImage to JPEG
        NSData *imgData = UIImageJPEGRepresentation(tempImage, 1); // 1 is compression quality
        
        // Identify the home directory and file name
        NSString *string = [NSString stringWithFormat:@"%ld.jpg",(long)indexPath.row];
        NSArray *documentDirectories = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
        NSString *documentDirectory = [[documentDirectories objectAtIndex:0] stringByAppendingPathComponent:string];
        
        
        // Write the file.  Choose YES atomically to enforce an all or none write. Use the NO flag if partially written files are okay which can occur in cases of corruption
        [imgData writeToFile:documentDirectory atomically:YES];
        
        [cell setBackgroundImage:tempImage];
        [delegate saveGraphWithSlotNum:(int)indexPath.row];
    }else{
        [delegate loadGraphWithSlotNum:(int)indexPath.row];
    }
}

+ (UIImage *)imageWithImage:(UIImage *)image scaledToSize:(CGSize)newSize {
    //UIGraphicsBeginImageContext(newSize);
    // In next line, pass 0.0 to use the current device's pixel scaling factor (and thus account for Retina resolution).
    // Pass 1.0 to force exact pixel size.
    UIGraphicsBeginImageContextWithOptions(newSize, NO, 0.0);
    [image drawInRect:CGRectMake(0, 0, newSize.width, newSize.height)];
    UIImage *newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return newImage;
}

- (UIImage *)snapshot:(UIView *)view

{
    
    UIGraphicsBeginImageContext(view.bounds.size);
    
    [view drawViewHierarchyInRect:view.bounds afterScreenUpdates:YES];
    
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    
    UIGraphicsEndImageContext();
    
    
    
    return image;
    
}

@end
