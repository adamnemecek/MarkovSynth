//
//  KnobControlView.m
//  AUPlayer
//
//  Created by Andrew Ayers on 3/2/14.
//
//

#import "KnobControlView.h"

@implementation KnobControlView
@synthesize _knobControl;
- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        _knobControl = [[RWKnobControl alloc] initWithFrame:self.bounds];
        [self addSubview:_knobControl];
        _knobControl.continuous = FALSE;
        _knobControl.lineWidth = 4.0;
        _knobControl.pointerLength = 8.0;
        self.tintColor = [UIColor redColor];
    }
    return self;
}

/*
// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    // Drawing code
}
*/

@end
