//
//  RLBiQuad.h
//  RLAudioEngine
//
//  Created by Andrew Ayers on 4/2/14.
//
//

#import "RLAudioEffect.h"

/** The type of filter to implement */
typedef NS_ENUM(NSInteger, RLBiQuadFilterType){
	RLBiQuadType_LP_First,	// 1st order LP type
    RLBiQuadType_LP_SecondButterworth,
    RLBiQuadType_HP_First,
    RLBiQuadType_HP_SecondButterworth,
    RLBiQuadType_BP_Second,
    RLBiQuadType_BP_SecondButterworth,
    RLBiQuadType_BS_Second,
    RLBiQuadType_BS_SecondButtorworth,
};

@interface RLBiQuad : RLAudioEffect{
    /** Filter Coefficients */
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
    
    /** Filter Coefficient calculation variables */
    float thetaC;
    float beta;
    float gamma;
    float C;
    float D;
    
    /** Left delay elements */
    float x_z1_L;
    float x_z2_L;
    float y_z1_L;
    float y_z2_L;
    
    /** Right delay elements */
    float x_z1_R;
    float x_z2_R;
    float y_z1_R;
    float y_z2_R;
    
    int counter;
}

/** The filter type being implemented in the filter currently */
@property (nonatomic) RLBiQuadFilterType filterType;

/** Cutoff frequency in Hz of the filter */
@property (nonatomic) float cutoff_hz;

/** Bandwidth in hertz, only used by certain filter types, qualityFactor is updated if this is updated, both are available so you can set however you want */
@property (nonatomic) float bandwidth_hz;

/** normal 'Q' value, 1/bandwidth_hz, bandwidth_hz is updated if this is updated and vice versa */
@property (nonatomic) float qualityFactor;

@end
