//
//  TouchDrawingView.h
//  ReLOADWireframe
//
//  Created by Andrew Ayers on 1/31/14.
//  Copyright (c) 2014 Rob Rehrig. All rights reserved.
//

#import <UIKit/UIKit.h>

@protocol touchDrawingViewDelegate <NSObject>

-(void)angleChanged;

@end

@interface TouchDrawingView : UIView{


}

-(void)clearBitmap;

@property double angle;
@property id delegate;

@end
