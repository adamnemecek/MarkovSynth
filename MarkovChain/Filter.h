#pragma once

#include "pluginconstants.h"
#include "fastapprox.h"
#include "synthfunctions.h"
#include "ModulationMatrix.h"

#define FILTER_FC_MOD_RANGE 60	//60 semitones = semitonesBetweenFrequencies(20.0, 20480.0)/2.0 = 120/2; 
#define FILTER_FC_MIN 20		//20Hz
#define FILTER_FC_MAX 20480		//20.480kHz = 10 octaves up from 20Hz

// CFilter Abastract Base Class for all filters
class CFilter
{
public:
	CFilter(void);
	~CFilter(void);
	
	CModulationMatrix* m_pModulationMatrix;
	
	UINT m_uModSourceFc;
	UINT m_uSourceFcControl; // direct control over Fc
	
	double m_dFcControl;	/* cutoff frequency control position */
	
	// for an aux filter specific like SEM BSF control
	double m_dAuxControl;

	// for NLP - Non Linear Procssing
	UINT m_uNLP;
	enum{OFF,ON}; 

	// for more grit
	double m_dSaturation;

	// NOTE: these are shared; even though some filters won't use some of them
	//       need to maintain the indexing
	/* filter selection */
	UINT m_uFilterType;		

	/* one short string for each */
	enum{LPF1,HPF1,LPF2,HPF2,BPF2,BSF2,LPF4,HPF4,BPF4};

protected:
	// common variables
	double m_dSampleRate;	/* sample rate*/

	// current variable used in calc
	double m_dFc;			/* cutoff frequency variable */

	// our cutoff frequency modulation input; 
	double m_dFcMod;		/* modulation input -1 to +1 */

	// Q or quality factor (resonance)
	double m_dQ;			/* Q */

public:
	// mutators
	//
	inline void setFcMod(double d){m_dFcMod = d;}
	inline void setSampleRate(double d){m_dSampleRate = d;}

	// flush buffers, reset filter
	virtual void reset();

	// decode the Q value
	virtual void setQControl(double dQControl);

	// pure virtual; derived classes MUST implement
	virtual double doFilter(double xn) = 0;

	inline virtual void updateFilter()
	{
		if(m_pModulationMatrix)
		{
			// NOTE: do NOT have to check for Source == DEST_NONE for mod inputs that
			//       have 0.0 as default (unity) value b/c destinations are cleared
			//       before each render
			m_dFcMod = m_pModulationMatrix->m_dDestinations[m_uModSourceFc];

			if(m_uSourceFcControl != DEST_NONE)
				m_dFcControl = m_pModulationMatrix->m_dDestinations[m_uSourceFcControl];
		}
		
		// do the modulation freq shift
		m_dFc = m_dFcControl*pitchShiftMultiplier(m_dFcMod);

		if(m_dFc > FILTER_FC_MAX)
			m_dFc = FILTER_FC_MAX;
		if(m_dFc < FILTER_FC_MIN)
			m_dFc = FILTER_FC_MIN;
	}
};
