//
//  AYACustomCell.m
//  
//
//  Created by Andrew Ayers on 4/11/14.
//
//

#import "AYACustomCell.h"

@implementation AYACustomCell

@synthesize mainLabel, detailLabel, slider;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        [self setBackgroundColor:[UIColor darkGrayColor]];
        
        
        mainLabel = [[UILabel alloc] initWithFrame:CGRectMake(10, 12, 150, 20)];
        mainLabel.highlightedTextColor = [UIColor whiteColor];
        mainLabel.textColor = [UIColor whiteColor];
        mainLabel.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:mainLabel];
        
        detailLabel = [[UILabel alloc] initWithFrame:CGRectMake(self.bounds.size.width-180, 12, 150, 20)];
        detailLabel.textColor = [UIColor whiteColor];
        detailLabel.text = [NSString stringWithFormat:@"%.2f",slider.value];
        detailLabel.highlightedTextColor = [UIColor whiteColor];
        detailLabel.backgroundColor = [UIColor clearColor];
        [self.contentView addSubview:detailLabel];
        
        slider = [[UISlider alloc] initWithFrame:CGRectMake(180, 12, 180, 0)];
        slider.alpha = 0;
        slider.maximumValue = 30;
        slider.autoresizingMask = UIViewAutoresizingFlexibleTopMargin;
        [self.contentView addSubview:slider];
        [slider addTarget:self action:@selector(sliderChanged:) forControlEvents:UIControlEventTouchDragInside];

    }
    return self;
}

- (void)setEditing:(BOOL)editing animated:(BOOL)animated
{
    [super setEditing:editing animated:animated];
    [UIView beginAnimations:nil context:NULL];
    [UIView setAnimationDuration:0.3];
    [UIView setAnimationDelegate:self];
    [UIView setAnimationBeginsFromCurrentState:YES];
    
    if(editing){
        slider.alpha = 1;
        slider.userInteractionEnabled=YES;
    }else{
        slider.alpha = 0;
        slider.userInteractionEnabled=NO;
    }
    
    [UIView commitAnimations];
}

-(IBAction) sliderChanged:(id) sender{
    detailLabel.text=[NSString stringWithFormat:@"%.2f",slider.value];
}


@end
