#pragma once
#include "fastapprox.h"
#include "pluginconstants.h"
#include "time.h"
#include <cmath>

#define CONVEX_LIMIT 0.00398107
#define CONCAVE_LIMIT 0.99601893

#define EXTRACT_BITS(the_val, bits_start, bits_len) ((the_val >> (bits_start - 1)) & ((1 << bits_len) - 1))


//----------------------------------------------------------------
// Constants
//----------------------------------------------------------------
// Status byte for Active Sensing message
const unsigned char ACTIVE_SENSING = 0xFE;

// Command value for Channel Pressure (Aftertouch)
const unsigned char CHANNEL_PRESSURE = 0xD0;

// Status byte for Continue message
const unsigned char CONTINUE = 0xFB;

// Command value for Control Change message (CC)
const unsigned char CONTROL_CHANGE = 0xB0;

// these are Data1 for CC
const unsigned char MOD_WHEEL = 0x01;
const unsigned char VOLUME_CC07 = 0x07;
const unsigned char PAN_CC10 = 0x0A;
const unsigned char EXPRESSION_CC11 = 0x0B;
const unsigned char SUSTAIN_PEDAL = 0x40;

// Status byte for System Exclusive message
const unsigned char SYSTEM_EXCLUSIVE = 0xF0;

// Status byte for End of System Exclusive message
const unsigned char END_OF_EXCLUSIVE = 0xF7;

// Status byte for MIDI Time Code Quarter Fram message
const unsigned char MIDI_TIME_CODE = 0xF1;

// Command value for Note On message
const unsigned char NOTE_ON = 0x90;

// Command value for Note Off message
const unsigned char NOTE_OFF = 0x80;
const unsigned char ALL_NOTES_OFF = 0x7E;

// Command value for Pitch Bend message
const unsigned char PITCH_BEND = 0xE0;

// Command value for Polyphonic Key Pressure (Aftertouch)
const unsigned char POLY_PRESSURE = 0xA0;

// Command value for Program Change message
const unsigned char PROGRAM_CHANGE = 0xC0;

// Status byte for Song Position Pointer message
const unsigned char SONG_POSITION_POINTER = 0xF2;

// Status byte for MIDI Song Select message
const unsigned char SONG_SELECT = 0xF3;

// Status byte for Start message
const unsigned char START = 0xFA;

// Status byte for Stop message
//const unsigned char STOP = 0xFC;

// Status byte for System Reset message
const unsigned char SYSTEM_RESET = 0xFF;

// Status byte for Timing Clock message
const unsigned char TIMING_CLOCK = 0xF8;

// Status byte for Tune Request message
const unsigned char TUNE_REQUEST = 0xF6;

enum midiChannels{MIDI_CH_1 = 0, MIDI_CH_2, MIDI_CH_3, MIDI_CH_4, MIDI_CH_5, MIDI_CH_6, MIDI_CH_7,
				  MIDI_CH_8, MIDI_CH_9, MIDI_CH_10, MIDI_CH_11, MIDI_CH_12, MIDI_CH_13,
				  MIDI_CH_14, MIDI_CH_15, MIDI_CH_16, MIDI_CH_ALL};

// --- Modulation Matrix Stuff --- //
enum sources{
	SOURCE_NONE = 0,
	SOURCE_LFO1,
	SOURCE_LFO2,
	SOURCE_LFO1Q,					/* QuadPhase outputs */
	SOURCE_LFO2Q,
	SOURCE_EG1,						/* Exponential outputs */
	SOURCE_EG2,
	SOURCE_EG3,
	SOURCE_EG4,
	SOURCE_BIASED_EG1,				/* Exponential outputs with Sustain subtracted*/
	SOURCE_BIASED_EG2,
	SOURCE_BIASED_EG3,
	SOURCE_BIASED_EG4,
	SOURCE_VELOCITY,	
	SOURCE_MIDI_VOLUME_CC07,		/* MIDI Volume */
	SOURCE_MIDI_PAN_CC10,			/* MIDI Pan */
	SOURCE_MIDI_EXPRESSION_CC11,	/* MIDI Expression */
	SOURCE_MODWHEEL,
	SOURCE_PITCHBEND,
	SOURCE_MIDI_NOTE_NUM, 
	MAX_SOURCES
};

enum destinations{
	DEST_NONE = 0,
	DEST_OSC1_FO,
	DEST_OSC2_FO,
	DEST_OSC3_FO,
	DEST_OSC4_FO,
	DEST_ALL_OSC_FO,
	DEST_OSC1_PULSEWIDTH,
	DEST_OSC2_PULSEWIDTH,
	DEST_OSC3_PULSEWIDTH,
	DEST_OSC4_PULSEWIDTH,
	DEST_ALL_OSC_PULSEWIDTH,
	DEST_OSC1_FO_RATIO,
	DEST_OSC2_FO_RATIO,
	DEST_OSC3_FO_RATIO,
	DEST_OSC4_FO_RATIO,
	DEST_ALL_OSC_FO_RATIO,
	DEST_OSC1_OUTPUT_AMP,
	DEST_OSC2_OUTPUT_AMP,
	DEST_OSC3_OUTPUT_AMP,
	DEST_OSC4_OUTPUT_AMP,
	DEST_ALL_OSC_OUTPUT_AMP,
	DEST_LFO1_FO,
	DEST_LFO2_FO,
	DEST_ALL_LFO_FO,
	DEST_LFO1_OUTPUT_AMP,
	DEST_LFO2_OUTPUT_AMP,
	DEST_ALL_LFO_OUTPUT_AMP,
	DEST_FILTER1_FC,
	DEST_FILTER2_FC,
	DEST_ALL_FILTER_FC,
	DEST_FILTER1_KEYTRACK,
	DEST_FILTER2_KEYTRACK,
	DEST_ALL_FILTER_KEYTRACK, // the control value, overriding the GUI
	DEST_FILTER1_Q,
	DEST_FILTER2_Q,
	DEST_ALL_FILTER_Q,
	DEST_EG1_ATTACK_SCALING,
	DEST_EG2_ATTACK_SCALING,
	DEST_EG3_ATTACK_SCALING,
	DEST_EG4_ATTACK_SCALING,
	DEST_ALL_EG_ATTACK_SCALING,
	DEST_EG1_DECAY_SCALING,
	DEST_EG2_DECAY_SCALING,
	DEST_EG3_DECAY_SCALING,
	DEST_EG4_DECAY_SCALING,
	DEST_ALL_EG_DECAY_SCALING,
	DEST_DCA_VELOCITY,
	DEST_DCA_EG,
	DEST_DCA_AMP,
	DEST_DCA_PAN,
	MAX_DESTINATIONS
};

enum transform{
	TRANSFORM_NONE = 0,
	TRANSFORM_UNIPOLAR_TO_BIPOLAR,
	TRANSFORM_BIPOLAR_TO_UNIPOLAR,
	TRANSFORM_MIDI_NORMALIZE,
	TRANSFORM_INVERT_MIDI_NORMALIZE,
	TRANSFORM_MIDI_TO_BIPOLAR,
	TRANSFORM_MIDI_TO_PAN,
	TRANSFORM_MIDI_TO_ATTENUATION,
	TRANSFORM_NOTE_NUMBER_TO_FREQUENCY,
	MAX_TRANSFORMS /* not needed? */
};

enum midiControls{
	MIDI_VOLUME,
	MIDI_PAN,
	MIDI_EXPRESSION,
	MIDI_MODWHEEL,
	MIDI_PITCHBEND,
	NUM_MIDI_CONTROLS 
};

struct modMatrixRow
{
	// index of source
	UINT uSourceIndex;

	// index of destination
	UINT uDestinationIndex;

	// needs to default to 1.0 in case no GUI control
	double* pModIntensity;	// <- "ucControl" in MMA DLS
	double* pModRange;		// <- "lScale" in MMA DLS

	// for enabling MIDI control smoothing
	bool bSmooth;
	double dZ1Register;

	UINT uSourceTransform;
};

struct noteRegion
{
	// note range of the Region
	UINT uMIDINoteLow;
	UINT uMIDINoteHigh;

	// velocity range of the Region
	UINT uMIDIVelocityLow;
	UINT uMIDIVelocityHigh;
};

inline modMatrixRow* createModMatrixRow(UINT uSource, UINT uDestination, double* pIntensity, double* pRange, UINT uTransform)
{
	modMatrixRow* pRow = new modMatrixRow;
	pRow->uSourceIndex = uSource;
	pRow->uDestinationIndex = uDestination;
	pRow->pModIntensity = pIntensity; // 0->1
	pRow->pModRange = pRange;
	pRow->uSourceTransform = uTransform; // for AmpMod

	return pRow;
}



inline void checkWrapIndex(double& dIndex)
{
	while(dIndex < 0.0)
		dIndex += 1.0;

	while(dIndex >= 1.0)
		dIndex -= 1.0;
}

inline double semitonesBetweenFrequencies(double dStartFrequency, double dEndFrequency)
{
	return fastlog2(dEndFrequency/dStartFrequency)*12.0;
//	return Log2(dEndFrequency/dStartFrequency)*12.0;
}

/* pitchShiftMultiplier()

	returns a multiplier for a given pitch shift in semitones
	to shift octaves,     call pitchShiftMultiplier(octaveValue*12.0);
	to shift semitones,   call pitchShiftMultiplier(semitonesValue);
	to shift cents,       call pitchShiftMultiplier(centsValue/100.0);
*/
inline double pitchShiftMultiplier(double dPitchShiftSemitones)
{
	if(dPitchShiftSemitones == 0)
		return 1.0;

	// 2^(N/12)
//	return fastpow2(dPitchShiftSemitones/12.0);
	return pow(2.0, dPitchShiftSemitones/12.0);
}

/* paramToTimeCents()

	converts a time parameter (secs) to TimeCents
	See the MMA MIDI DLS Level 1 or 2 Spec

*/
inline double paramToTimeCents(double dParam)
{
	return 1200.0*65536.0*fastlog2(dParam);
}

/* timeCentsToParam()
	
	converts a timeCents to a time (sec) value
	See the MMA MIDI DLS Level 1 or 2 Spec

*/
inline double timeCentsToParam(double dTimeCents)
{
	return fastpow2(dTimeCents/(1200.0*65536.0));
}

/* mmaMIDItoParamScaling()
	
	calculates the scaled version of a parameter based on a MIDI value
	See the MMA MIDI DLS Level 1 or 2 Spec
	
	uMIDIValue = the MIDI (0 -> 127) value to apply
	fParameter = the parameter to scale
	fScaling = the scaling amount, 0 -> 1.0

	Examples:
		Velocity to EG Attack Time: attack time decreases as velocity increases
									uMIDIValue = velocity
									fParameter = attack time (mSec)
									fScaling = Vel to EG Attack Scale

		Key to EG Decay Time: decay shortens as MIDI note number increases
							  uMIDIValue = MIDI note number
							  fParameter = decay time (mSec)
							  fScaling = Vel to EG Attack Scale


*/
inline double mmaMIDItoParamScaling(UINT uMIDIValue, float fParameter, float fScaling)
{
	return timeCentsToParam(paramToTimeCents(fScaling)*(double(uMIDIValue)/128.0) + paramToTimeCents(fParameter));
}

/* mmaMIDItoAtten_dB()
	
	calculates the dB of attenuation according to MMA DLS spec
	
	uMIDIValue = the MIDI (0 -> 127) value to convert
*/
inline double mmaMIDItoAtten_dB(UINT uMIDIValue)
{
	if(uMIDIValue == 0)
		return -96.0; // dB floor

	return 20.0*log10((127.0*127.0)/((float)uMIDIValue*(float)uMIDIValue));
}

/* mmaMIDItoAtten()
	
	calculates the raw attenuation according to MMA DLS spec
	
	uMIDIValue = the MIDI (0 -> 127) value to convert
*/
inline double mmaMIDItoAtten(UINT uMIDIValue)
{
	if(uMIDIValue == 0)
		return 0.0; // floor

	return ((double)uMIDIValue*(double)uMIDIValue)/(127.0*127.0);;
}

/* capChargeTransform()
	
	calculates the expDecayTransform of the input

	dValue = the unipolar (0 -> 1) value to convert

	returns a value from 0 up to 1.0
*/
inline double capChargeTransform(double dValue)
{
	if(dValue == 0.0)
		return 0.0;

	return 1.0 - exp(-6.0*dValue);
}

/* capDischargeTransform()
	
	calculates the capDischargeTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double capDischargeTransform(double dValue)
{
	if(dValue == 1.0)
		return 0.0;

	return exp(-6.0*dValue);
}


/* linearIn_dBTransform()
	
	calculates the linearIn_dBTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double linearIn_dB_AttackTransform(double dValue)
{
	if(dValue == 0.0)
		return 0.0;

	return pow(10.0, ((1.0 - dValue)*-96.0)/20.0);
}

/* linearIn_dBTransform()
	
	calculates the linearIn_dBTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double linearIn_dB_DecayTransform(double dValue)
{
	if(dValue == 1.0)
		return 0.0;

	return pow(10.0, (dValue*-96.0)/20.0);
}


/* convexTransform()
	
	calculates the convexTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double convexTransform(double dValue)
{
	if(dValue <= CONVEX_LIMIT)
		return 0.0;

	return 1.0 + (5.0/12.0)*log10(dValue);
}

/* convexInvertedTransform()
	
	calculates the convexInvertedTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double convexInvertedTransform(double dValue)
{
	if(dValue >= CONCAVE_LIMIT)
		return 0.0;

	return 1.0 + (5.0/12.0)*log10(1.0 - dValue);
}

/* concaveTransform()
	
	calculates the concaveTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double concaveTransform(double dValue)
{
	if(dValue >= CONCAVE_LIMIT)
		return 1.0;

	return -(5.0/12.0)*log10(1.0 - dValue);
}

/* concaveInvertedTransform()
	
	calculates the concaveInvertedTransform of the input
	
	dValue = the unipolar (0 -> 1) value to convert
*/
inline double concaveInvertedTransform(double dValue)
{
	if(dValue <= CONVEX_LIMIT)
		return 1.0;

	return -(5.0/12.0)*log10(dValue);
}

/* unipolarToBipolar()
	
	calculates the bipolar (-1 -> +1) value from a unipolar (0 -> 1) value
	
	dValue = the value to convert
*/
inline double unipolarToBipolar(double dValue)
{
	return 2.0*dValue - 1.0;
}

/* midiToBipolar()
	
	calculates the bipolar (-1 -> +1) value from a unipolar MIDI (0 -> 127) value
	
	UINT uMIDIValue = the MIDI value to convert
*/
inline double midiToBipolar(UINT uMIDIValue)
{
	return 2.0*(double)uMIDIValue/127.0 - 1.0;
}

/* midiToPanValue()
	
	calculates the pan value (-1 -> +1) value from a unipolar MIDI (0 -> 127) value
	
	UINT uMIDIValue = the MIDI value to convert
*/
inline double midiToPanValue(UINT uMIDIValue)
{
	// see MMA DLS Level 2 Spec; controls are asymmetrical
	if(uMIDIValue == 64)
		return 0.0;
	else if (uMIDIValue <= 1) // 0 or 1
		return -1.0;

	return 2.0*(double)uMIDIValue/127.0 - 1.0;
}

/* bipolarToUnipolar()
	
	calculates the unipolar (0 -> 1) value from a bipolar (-1 -> +1) value
	
	dValue = the value to convert
*/
inline double bipolarToUnipolar(double dValue)
{
	return 0.5*dValue + 0.5;
}

/* midiToUnipolar()
	
	calculates the unipolar (0 -> 1) value from a MIDI (0 -> 127) value
	
	dValue = the value to convert
*/
inline double midiToUnipolar(UINT uMIDIValue)
{
	return (double)uMIDIValue/127.0;
}

/* calculatePanValues()
	
	calculates the left and right pan values
	
	dPanMod = bipolar (-1 -> 1) pan modulation value

	returns are via pass-by-reference mechanism
*/
inline void calculatePanValues(double dPanMod, double& dLeftPanValue, double& dRightPanValue)
{
	dLeftPanValue = fastcos((pi/4.0)*(dPanMod + 1.0));
	dRightPanValue = fastsin((pi/4.0)*(dPanMod + 1.0));

	dLeftPanValue = fmax(dLeftPanValue, 0.0);
	dLeftPanValue = fmin(dLeftPanValue, 1.0);
	dRightPanValue = fmax(dRightPanValue, 0.0);
	dRightPanValue = fmin(dRightPanValue, 1.0);
}

/* calcValueVoltOctaveExp()
	
	returns the volt/octave exponential value
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = on the range of 0 -> 1
*/
inline double calcValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue)
{
	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	if(dLowLimit == 0)
		return dControlValue;
	
	// exp control
	return dLowLimit*fastpow2(dControlValue*dOctaves);
}

/* calcInverseValueVoltOctaveExp()
	
	returns a value between 0 -> 1 representing the volt/octave control location
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = value between low and high limit
*/
inline double calcInverseValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue)
{
	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	if(dLowLimit == 0)
		return dControlValue;
	
	return fastlog2(dControlValue/dLowLimit)/dOctaves;
}

inline double log2( double n )  
{  
    // log(n)/log(2) is log2.  
    return log( n ) / log((double)2 );  
}


/* calcModulatedValueExp()
	
	returns the new exponentially modulated control value
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = current value of control
	dModValue = moudulator: on the range of 0 -> 1
	bExpUserControl = type of control on GUI; TRUE if user is adjusting a volt/octave control FALSE if linear
*/
inline double calcModulatedValueVoltOctaveExp(double dLowLimit, double dHighLimit, double dControlValue, 
											  double dModValue, bool bExpUserControl)
{
	dModValue = bipolarToUnipolar(dModValue);

//	double dOctaves = fastlog2(dHighLimit/dLowLimit);
	double dOctaves = log2(dHighLimit/dLowLimit);
	
	// location of control relative to 50% point
	double dControlOffset = bExpUserControl ? calcInverseValueVoltOctaveExp(dLowLimit, dHighLimit,dControlValue) - 0.5 : ((dControlValue - dLowLimit)/(dHighLimit - dLowLimit)) - 0.5;

	// exp control
//	double dExpControlOffset = dLowLimit*fastpow2(dControlOffset*dOctaves);
	double dExpControlOffset = dLowLimit*pow(2.0, dControlOffset*dOctaves);
	
	// mod exp control
	double dModulatedValueExp = dExpControlOffset*pow(2.0, dModValue*dOctaves);

	// bound
	dModulatedValueExp = min(dModulatedValueExp, dHighLimit);
	dModulatedValueExp = max(dModulatedValueExp, dLowLimit);

	// return it
	return dModulatedValueExp;
}

/* calcModulatedValueLin()
	
	returns the new linearly modulated control value
	
	dLowLimit = low limit of control
	dHighLimit = high limit of control
	dControlValue = current value of control
	dModValue = moudulator: on the range of 0 -> 1
*/
inline double calcModulatedValueLin(double dLowLimit, double dHighLimit, double dControlValue, double dModValue)
{	
	// convert mod value to bipolar
	double dModulator = unipolarToBipolar(dModValue)*(dHighLimit - dLowLimit)/2.0;// + dLowLimit; 
	
	double dModulatedValue = dControlValue + dModulator;

	// bound
	dModulatedValue = min(dModulatedValue, dHighLimit);
	dModulatedValue = max(dModulatedValue, dLowLimit);

	// return it
	return dModulatedValue;
}

/* polyBLEP()
	
	returns the PolyBLEP residual for a given sample
	
	dModulo = modulo counter value (0->1)
	dInc = phase increment
	dHeight = height of discontinuity (0->1)
	bRisingEdge = true for rising edge, false for falling
	bDone = pass by reference flag telling caller PolyBLEP did the second sample and is done for this discont.
	dEdgeLocation = location of the discontinuity between 0 and 1; 
					for Sawtooth the edge is at 1.0, falling
					for Square the rising edge is at 1.0 and the falling edge is at 0.5
*/
inline double polyBLEP(double dModulo, double dInc, double dHeight, bool bRisingEdge, bool& bDone, double dEdgeLocation = 0.0)
{
	// bDone is a flag letting the caller know we processed 
	// the point on the right side of the discontinuity
	bDone = false;

	// return value
	double dPolyBLEP = 0.0;

	// t = the distance from the discontinuity
	double t = 0.0;
	
	// LEFT side of edge
	// -1 < t < 0
	if((dEdgeLocation == 0 && dModulo > 1.0 - dInc) || 
		((dModulo > dEdgeLocation - dInc) && dModulo < dEdgeLocation))
	{ 
		// for the wrap-around edge only
		if(dEdgeLocation == 0.0)
			dEdgeLocation = 1.0;
		
		// calculate distance
		t = (dModulo - dEdgeLocation)/dInc; 

		// calculate residual
		dPolyBLEP = dHeight*(t*t + 2.0*t + 1.0); 
//		dPolyBLEP = dHeight*(t*t/2.0 + t + 0.5); 
	
		// subtract for falling, add for rising edge
		if(!bRisingEdge)
			dPolyBLEP *= -1.0;

		return dPolyBLEP;
	} 

	// RIGHT side of discontinuity
	// 0 <= t < 1 
	if(dModulo < dEdgeLocation + dInc && dModulo > dEdgeLocation)
	{ 
		// set done flag
		bDone = true;

		// calculate distance
		t = (dModulo - dEdgeLocation)/dInc; 

		// calculate residual
		dPolyBLEP = dHeight*(2.0*t - t*t - 1.0); 
//		dPolyBLEP = dHeight*(t - t*t/2.0 - 0.5); 
	
		// subtract for falling, add for rising edge
		if(!bRisingEdge)
			dPolyBLEP *= -1.0;

		return dPolyBLEP;
	} 

	return 0.0; 
} 

inline double doWhiteNoise()
{
	// fNoise is 0 -> 32767.0
	float fNoise = (float)rand();

	// normalize and make bipolar
	fNoise = 2.0*(fNoise/32767.0) - 1.0;

	return fNoise;
}

inline double doPNSequence(UINT& uPNRegister)
{
	// get the bits
	UINT b0 = EXTRACT_BITS(uPNRegister, 1, 1); // 1 = b0 is FIRST bit from right
	UINT b1 = EXTRACT_BITS(uPNRegister, 2, 1); // 1 = b1 is SECOND bit from right
	UINT b27 = EXTRACT_BITS(uPNRegister, 28, 1); // 28 = b27 is 28th bit from right
	UINT b28 = EXTRACT_BITS(uPNRegister, 29, 1); // 29 = b28 is 29th bit from right

	// form the XOR
	UINT b31 = b0^b1^b27^b28;

	// form the mask to OR with the register to load b31
	if(b31 == 1)
		b31 = 0x10000000;

	// shift one bit to right
	uPNRegister >>= 1; 

	// set the b31 bit
	uPNRegister |= b31;

	// convert the output into a floating point number, scaled by experimentation
	// to a range of o to +2.0
	float fOut = (float)(uPNRegister)/((pow((float)2.0,(float)32.0))/16.0);

	// shift down to form a result from -1.0 to +1.0
	fOut -= 1.0;

	return fOut;
}

const double B = 4/pi;
const double C = -4/(pi*pi);
const double P = 0.225;
// http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
// input is -pi to +pi
inline double parabolicSine(double x, bool bHighPrecision = true)
{
    double y = B * x + C * x * fabs(x);

    if(bHighPrecision)
        y = P * (y * fabs(y) - y) + y;

	return y;
}

const double D = 5.0*pi*pi;
inline double BhaskaraISine(double x)
{
	double sgn = x/fabs(x);
	return 16.0*x*(pi - sgn*x)/(D - sgn*4.0*x*(pi - sgn*x));
}

//inline void copyWaveData(CWaveData* pSourceWaveData, CWaveData* pDestWaveData)
//{
//	if(!pSourceWaveData) return;
//
//	pDestWaveData->m_pWaveBuffer = new float[pSourceWaveData->m_uSampleCount];
//
//	// copy everything
//	for(int i=0; i<pSourceWaveData->m_uSampleCount; i++)
//	{
//		pDestWaveData->m_pWaveBuffer[i] = pSourceWaveData->m_pWaveBuffer[i];
//		pDestWaveData->m_bWaveLoaded = pSourceWaveData->m_bWaveLoaded;
//		pDestWaveData->m_uNumChannels = pSourceWaveData->m_uNumChannels;
//		pDestWaveData->m_uSampleRate = pSourceWaveData->m_uSampleRate;
//		pDestWaveData->m_uSampleCount = pSourceWaveData->m_uSampleCount;
//		pDestWaveData->m_uLoopCount = pSourceWaveData->m_uLoopCount;
//		pDestWaveData->m_uLoopStartIndex = pSourceWaveData->m_uLoopStartIndex;
//		pDestWaveData->m_uLoopEndIndex = pSourceWaveData->m_uLoopEndIndex;
//		pDestWaveData->m_uLoopType = pSourceWaveData->m_uLoopType;
//		pDestWaveData->m_uMIDINote = pSourceWaveData->m_uMIDINote;
//		pDestWaveData->m_uMIDIPitchFraction = pSourceWaveData->m_uMIDIPitchFraction;
//		pDestWaveData->m_uSMPTEFormat = pSourceWaveData->m_uSMPTEFormat;
//		pDestWaveData->m_uSMPTEOffset = pSourceWaveData->m_uSMPTEOffset;
//	}
//}

const float midiMIDIFreqTable[128] = {
    8.1757993698120117,
    8.6619567871093750,
    9.1770238876342773,
    9.7227182388305664,
    10.3008613586425780,
    10.9133825302124020,
    11.5623254776000980,
    12.2498569488525390,
    12.9782714843750000,
    13.7500000000000000,
    14.5676174163818360,
    15.4338531494140630,
    16.3515987396240230,
    17.3239135742187500,
    18.3540477752685550,
    19.4454364776611330,
    20.6017227172851560,
    21.8267650604248050,
    23.1246509552001950,
    24.4997138977050780,
    25.9565429687500000,
    27.5000000000000000,
    29.1352348327636720,
    30.8677062988281250,
    32.7031974792480470,
    34.6478271484375000,
    36.7080955505371090,
    38.8908729553222660,
    41.2034454345703130,
    43.6535301208496090,
    46.2493019104003910,
    48.9994277954101560,
    51.9130859375000000,
    55.0000000000000000,
    58.2704696655273440,
    61.7354125976562500,
    65.4063949584960940,
    69.2956542968750000,
    73.4161911010742190,
    77.7817459106445310,
    82.4068908691406250,
    87.3070602416992190,
    92.4986038208007810,
    97.9988555908203130,
    103.8261718750000000,
    110.0000000000000000,
    116.5409393310546900,
    123.4708251953125000,
    130.8127899169921900,
    138.5913085937500000,
    146.8323822021484400,
    155.5634918212890600,
    164.8137817382812500,
    174.6141204833984400,
    184.9972076416015600,
    195.9977111816406200,
    207.6523437500000000,
    220.0000000000000000,
    233.0818786621093700,
    246.9416503906250000,
    261.6255798339843700,
    277.1826171875000000,
    293.6647644042968700,
    311.1269836425781200,
    329.6275634765625000,
    349.2282409667968700,
    369.9944152832031200,
    391.9954223632812500,
    415.3046875000000000,
    440.0000000000000000,
    466.1637573242187500,
    493.8833007812500000,
    523.2511596679687500,
    554.3652343750000000,
    587.3295288085937500,
    622.2539672851562500,
    659.2551269531250000,
    698.4564819335937500,
    739.9888305664062500,
    783.9908447265625000,
    830.6093750000000000,
    880.0000000000000000,
    932.3275146484375000,
    987.7666015625000000,
    1046.5023193359375000,
    1108.7304687500000000,
    1174.6590576171875000,
    1244.5079345703125000,
    1318.5102539062500000,
    1396.9129638671875000,
    1479.9776611328125000,
    1567.9816894531250000,
    1661.2187500000000000,
    1760.0000000000000000,
    1864.6550292968750000,
    1975.5332031250000000,
    2093.0046386718750000,
    2217.4609375000000000,
    2349.3181152343750000,
    2489.0158691406250000,
    2637.0205078125000000,
    2793.8259277343750000,
    2959.9553222656250000,
    3135.9633789062500000,
    3322.4375000000000000,
    3520.0000000000000000,
    3729.3100585937500000,
    3951.0664062500000000,
    4186.0092773437500000,
    4434.9218750000000000,
    4698.6362304687500000,
    4978.0317382812500000,
    5274.0410156250000000,
    5587.6518554687500000,
    5919.9106445312500000,
    6271.9267578125000000,
    6644.8750000000000000,
    7040.0000000000000000,
    7458.6201171875000000,
    7902.1328125000000000,
    8372.0185546875000000,
    8869.8437500000000000,
    9397.2724609375000000,
    9956.0634765625000000,
    10548.0820312500000000,
    11175.3037109375000000,
    11839.8212890625000000,
	12543.8535156250000000};
