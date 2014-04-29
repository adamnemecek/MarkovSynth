//
//  AYAAppDelegate.m
//  MarkovChain
//
//  Created by Andrew Ayers on 4/10/14.
//  Copyright (c) 2014 AyersAudio. All rights reserved.
//

#import "AYAAppDelegate.h"


@implementation AYAAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
// Test code for midi file read in.
    
    
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen]
                                                   bounds]];
    // Override point for customization after application launch.
    TRPLeftDrawerViewController *masterViewController = [[TRPLeftDrawerViewController alloc] init];
    UINavigationController *masterNavigationController =
    [[UINavigationController alloc] initWithRootViewController:
     masterViewController];
    
    AYAMainViewController *detailViewController = [[AYAMainViewController alloc] init];
    UINavigationController *detailNavigationController =
    [[UINavigationController alloc] initWithRootViewController:
     detailViewController];
    
    [masterViewController setMainVC:detailViewController];
    self.splitViewController = [[UISplitViewController alloc] init];
    self.splitViewController.delegate = detailViewController;
    self.splitViewController.viewControllers =
    @[masterNavigationController, detailNavigationController];
    self.window.rootViewController = self.splitViewController;
    [self.splitViewController setPresentsWithGesture:YES];

    [self.window makeKeyAndVisible];
    return YES;
}
-(BOOL)splitViewController:(UISplitViewController *)svc shouldHideViewController:(UIViewController *)vc inOrientation:(UIInterfaceOrientation)orientation
{
    return YES;
}

- (TRPLeftDrawerViewController*)leftDrawerViewController
{
    if (_leftDrawerViewController) {
        return _leftDrawerViewController;
    }
    
    _leftDrawerViewController = [[TRPLeftDrawerViewController alloc] initWithStyle:UITableViewStyleGrouped];
    [_leftDrawerViewController setMainVC:self.mainVC];
    return _leftDrawerViewController;
}
- (BOOL)dynamicsDrawerViewController:(MSDynamicsDrawerViewController *)drawerViewController
                  shouldBeginPanePan:(UIPanGestureRecognizer *)panGestureRecognizer
{
    UINavigationController *rootNavigationController = (UINavigationController*)drawerViewController.paneViewController;
    return [rootNavigationController.viewControllers count] == 1;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
