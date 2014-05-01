//
//  RLBiQuad.m
//  RLAudioEngine
//
//  Created by Andrew Ayers on 4/2/14.
//
//

#import "RLBiQuad.h"

@implementation RLBiQuad

@synthesize filterType = _filterType;
@synthesize cutoff_hz = _cutoff_hz;
@synthesize bandwidth_hz = _bandwidth_hz;
@synthesize qualityFactor = _qualityFactor;

- (id)init
{
    if((  self = [super init])){
        //   if (self) {
        
        counter = 0;
        
        a0 = 0.0;
        a1 = 0.0;
        a2 = 0.0;
        b1 = 0.0;
        b2 = 0.0;
        
        thetaC = 0.0;
        beta = 0.0;
        C = 0.0;
        D = 0.0;
        gamma = 0.0;
        
        x_z1_L = 0.0;
        x_z2_L = 0.0;
        y_z1_L = 0.0;
        y_z2_L = 0.0;
        
        x_z1_R = 0.0;
        x_z2_R = 0.0;
        y_z1_R = 0.0;
        y_z2_R = 0.0;
        
        // init to no filtering
        _cutoff_hz = 600.0;
        _bandwidth_hz = 5000.0;
        // filterType = RLBiQuadType_HP_First;
        _filterType = RLBiQuadType_LP_First;
        [self setFilterCoefficients];
        
    }
    return self;
}


-(void)setFilterType:(RLBiQuadFilterType)filterType{
    if (_filterType != filterType) {
        _filterType = filterType;
    }
    [self setFilterCoefficients];
}

-(void)setCutoff_hz:(float)cutoff_hz{
    if (_cutoff_hz != cutoff_hz) {
        _cutoff_hz = cutoff_hz;
    }
    [self setFilterCoefficients];
}

-(void)setBandwidth_hz:(float)bandwidth_hz{
    if (_bandwidth_hz != bandwidth_hz) {
        _bandwidth_hz = bandwidth_hz;
    }
    [self setFilterCoefficients];
}

-(void)setQualityFactor:(float)qualityFactor{
    if (_qualityFactor != qualityFactor) {
        _qualityFactor = qualityFactor;
    }
    [self setFilterCoefficients];
}


-(void)prepareForPlay{
    [self setFilterCoefficients];
}

- (void) setFilterCoefficients
{
    switch ( _filterType ) {
            
            /** Low Pass Filters */
        case RLBiQuadType_LP_First:
            thetaC = 2*M_PI*(float)self.sampleRate/_cutoff_hz;
            gamma = cosf( thetaC ) / ( 1 + sinf( thetaC ) ) ;
            a0 = ( 1.0 - gamma ) / 2.0;
            a1 =  a0;
            a2 = 0.0;
            b1 = -1*gamma;
            b2 = 0.0;
            break;
            
        case RLBiQuadType_LP_SecondButterworth:
            C = 1.0 / (tanf(M_PI*_cutoff_hz / (float)self.sampleRate));
            a0 = 1.0 / ( 1.0 + C*sqrtf( 2.0 ) + C*C);
            a1 = 2.0*a0;
            a2 = a0;
            b1 = 2.0*a0* ( 1 - C*C );
            b2 = a0* ( 1.0 - C*sqrtf( 2.0 ) + C*C );
            break;
            
            /** High Pass Filters */
        case RLBiQuadType_HP_First:
            thetaC = 2*M_PI*(float)self.sampleRate/_cutoff_hz;
            gamma = cosf( thetaC ) / ( 1 + sinf( thetaC ) ) ;
            a0 = ( 1.0 + gamma ) / 2.0;
            a1 =  -1*a0;
            a2 = 0.0;
            b1 = -1*gamma;
            b2 = 0.0;
            break;
            
        case RLBiQuadType_HP_SecondButterworth:
            C = tanf(M_PI*_cutoff_hz / (float)self.sampleRate);
            a0 = 1.0 / ( 1.0 + C*sqrtf( 2.0 ) + C*C);
            a1 = -2.0*a0;
            a2 = a0;
            b1 = 2.0*a0* ( C*C - 1.0 );
            b2 = a0* ( 1.0 - C*sqrtf( 2.0 ) + C*C );
            break;
            
            /** Band Pass Filters */
        case RLBiQuadType_BP_Second:
            thetaC = 2*M_PI*(float)self.sampleRate/_cutoff_hz;
            beta = 0.5 * ( ( 1 - tanf(thetaC*_bandwidth_hz/2.0) ) / ( 1 + tanf(thetaC*_bandwidth_hz/2.0) ) );
            gamma = ( 0.5 + beta ) * cosf(thetaC);
            a0 = 0.5 - beta;
            a1 =  0.0;
            a2 = -1.0*a0;
            b1 = -2.0*gamma;
            b2 = 2.0*beta;
            break;
            
        case RLBiQuadType_BP_SecondButterworth:
            C = 1.0 / ( tanf(M_PI * _bandwidth_hz / (float)self.sampleRate) );
            //            NSLog(@"%f, %f", cutoff_hz, bandwidth_hz);
            D = 2.0 * cosf(2.0 * M_PI * _cutoff_hz / (float)self.sampleRate);
            a0 = 1.0 / ( 1.0 + C );
            a1 = 0.0;
            a2 = -1.0 * a0;
            b1 = a2 * C * D;
            b2 = a0 * ( C - 1.0 );
            break;
            
            /** Band Stop Filters */
        case RLBiQuadType_BS_Second:
            thetaC = 2*M_PI*(float)self.sampleRate/_cutoff_hz;
            beta = 0.5 * ( ( 1 - tanf(thetaC*_bandwidth_hz/2.0) ) / ( 1 + tanf(thetaC*_bandwidth_hz/2.0) ) );
            gamma = ( 0.5 + beta ) * cosf(thetaC);
            a0 = 0.5 + beta;
            a1 =  -2.0 * gamma;
            a2 = 0.5 + beta;
            b1 = -2.0 * gamma;
            b2 = 2.0 * beta;
            break;
            
        case RLBiQuadType_BS_SecondButtorworth:
            C = tanf(M_PI * _bandwidth_hz / (float)self.sampleRate);
            D = 2.0 * cosf(2.0 * M_PI * _cutoff_hz / (float)self.sampleRate);
            a0 = 1.0 / ( 1.0 + C );
            a1 = -1.0 * a0 * D;
            a2 = a0;
            b1 = -1.0 * a0 * D;
            b2 = a0 * ( 1.0 - C );
            break;
            
        default:
            break;
    }
}

- (void) setParameterValue:(NSNumber *)value forKey:(NSString *)key
{
    if ( [ key isEqualToString:@"cutoff_hz" ] )
    {
        _cutoff_hz = [value floatValue];
        [self setFilterCoefficients];
    }
    else if ( [ key isEqualToString:@"bandwidth_hz" ] )
    {
        _bandwidth_hz = [value floatValue];
        _qualityFactor = 1.0/_bandwidth_hz;
        [self setFilterCoefficients];
    }
    else if ( [ key isEqualToString:@"qualityFactor" ] )
    {
        _qualityFactor = [value floatValue];
        _bandwidth_hz = 1.0/_qualityFactor;
        [self setFilterCoefficients];
    }
}

- (void) processAudioFrameInPlace:(float *)frame
{

    // copy to temp location in case pInput and pOutput point to the same address ( in which case we'd loose these inputs after the difference equation is evaluated
    float tmpL = frame[0];
    float tmpR = frame[1];
    
    if(isnan(tmpL) || tmpL== INFINITY){
        tmpL = 0.0;
    }
    if(isnan(tmpR) || tmpR == INFINITY){
        tmpR = 0.0;
    }
    if(isnan(a0) || a0 == INFINITY){
        a0 = 0.0;
    }
    if(isnan(a1) || a1 == INFINITY){
        a1 = 0.0;
    }
    if(isnan(a2) || a2 == INFINITY){
        a2 = 0.0;
    }
    if(isnan(b1) || b1 == INFINITY){
        b1 = 0.0;
    }
    if(isnan(b2) || b2 == INFINITY){
        b2 = 0.0;
    }
    //
    if(isnan(thetaC)|| thetaC == INFINITY){
        thetaC = 0.0;
    }
    if(isnan(beta)|| beta == INFINITY){
        beta = 0.0;
    }
    if(isnan(C)|| C == INFINITY){
        C = 0.0;
    }
    if(isnan(D)|| D == INFINITY){
        D = 0.0;
    }
    if(isnan(gamma)|| gamma== INFINITY){
        gamma = 0.0;
    }
    
    //    if ( fabsf(frame[0]) > 1.0 || fabsf(frame[1]) > 1.0)
    //    {
    //        int i = 1;
    //    }
    
    frame[0] = a0*tmpL + a1*x_z1_L + a2*x_z2_L - b1*y_z1_L - b2*y_z2_L;
    frame[1] = a0*tmpR + a1*x_z1_R + a2*x_z2_R - b1*y_z1_R - b2*y_z2_R;
    
    if (counter == 1024)
    {
        //        NSLog(@"%f, %f", frame[0], frame[1]);
        counter = 0;
    }
    
    //    if ( fabsf(frame[0]) > 1.0 || fabsf(frame[1]) > 1.0)
    //    {
    //        int i = 1;
    //    }
    
    x_z2_L = x_z1_L;
    x_z2_R = x_z1_R;
    
    x_z1_L = tmpL;
    x_z1_R = tmpR;
    
    y_z2_L = y_z1_L;
    y_z2_R = y_z1_R;
    y_z1_L = frame[0];
    y_z1_R = frame[1];
    
    if(isnan(y_z1_L)){
        y_z1_L= 0.0;
    }
    if(isnan(y_z1_L)){
        y_z2_L= 0.0;
    }
    
    if(isnan(y_z1_R)){
        y_z1_R= 0.0;
    }
    
    if(isnan(y_z2_R)){
        y_z2_R= 0.0;
    }
    
    
    
    //    a1 = 0.0;
    //    a2 = 0.0;
    //    b1 = 0.0;
    //    b2 = 0.0;
    
    counter++;
}


@end
