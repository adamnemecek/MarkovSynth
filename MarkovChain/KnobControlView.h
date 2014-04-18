//
//  KnobControlView.h
//  AUPlayer
//
//  Created by Andrew Ayers on 3/2/14.
//
//

#import <UIKit/UIKit.h>
#import "RWKnobControl.h"

@interface KnobControlView : UIView{
    RWKnobControl *_knobControl;
}

@property RWKnobControl* _knobControl;
@end
