#pragma once
#include "synthfunctions.h"
#include <list>
#include <vector>

using namespace std;

class CModulationMatrix
{
public:
	CModulationMatrix(void);
	~CModulationMatrix(void);

protected:
	// the acutal matrix of rows
	std::vector<modMatrixRow*> m_ModMatrix;
	std::vector<modMatrixRow*>::iterator m_MatrixIterator;

	int m_nSize;
public:
	// sources: where each source (LFO, EG, ETC) writes its output value
	double m_dSources[MAX_SOURCES];

	// destinations: where the newly calculated mod values are read from
	double m_dDestinations[MAX_DESTINATIONS];
	
	// this is a simple Forward Integrator
	inline double midiCtrlSmoother(modMatrixRow* p)
	{
		double xn = m_dSources[p->uSourceIndex];
		double yn = p->dZ1Register + 0.001*(xn - p->dZ1Register);
		p->dZ1Register = yn;
		return yn;
	}

	// go through each row and transfer/accumulate sources into destinations
	// destination += source*intensity*range
	inline void doModulationMatrix()
	{
		// clear dest registers
		clearDestinations();

		for(int i=0; i<m_nSize; i++)
		{
			// get the row
			modMatrixRow* p = m_ModMatrix[i];

			// get the source value
			double dSource = m_dSources[p->uSourceIndex];
			
			// apply MIDI smoothing
			//if(p->bSmooth)
			//	dSource = midiCtrlSmoother(p);

			switch(p->uSourceTransform)
			{				
				case TRANSFORM_UNIPOLAR_TO_BIPOLAR:
					dSource = unipolarToBipolar(dSource);
					break;
				
				case TRANSFORM_BIPOLAR_TO_UNIPOLAR:
					dSource = bipolarToUnipolar(dSource);
					break;
			
				case TRANSFORM_MIDI_TO_ATTENUATION:
					dSource = mmaMIDItoAtten(dSource);
					break;
			
				case TRANSFORM_MIDI_TO_PAN:
					dSource = midiToPanValue(dSource);
					break;
			
				case TRANSFORM_MIDI_TO_BIPOLAR:
					dSource = midiToBipolar(dSource);
					break;

				case TRANSFORM_NOTE_NUMBER_TO_FREQUENCY:
					dSource = midiMIDIFreqTable[(UINT)dSource];
					break;
			
				case TRANSFORM_MIDI_NORMALIZE:
					dSource/= (double)127.0; // 0->1 NOTE: MMA DLS uses divide-by-128 instead!, 0->0.9999
					break;
				
				case TRANSFORM_INVERT_MIDI_NORMALIZE:
					dSource/= (double)127.0; // 0->1 NOTE: MMA DLS uses divide-by-128 instead!, 0->0.9999
					dSource = 1.0 - dSource; // 1->0 NOTE: MMA DLS uses divide-by-128 instead!, 0.9999->0
					break;

				default:
					break;
			}

			// destination += source*intensity*range
			//
			// first check DEST_ALL types
			switch(p->uDestinationIndex)
			{
				case DEST_ALL_OSC_FO:
					m_dDestinations[DEST_OSC1_FO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC2_FO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC3_FO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC4_FO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_OSC_PULSEWIDTH:
					m_dDestinations[DEST_OSC1_PULSEWIDTH] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC2_PULSEWIDTH] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC3_PULSEWIDTH] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC4_PULSEWIDTH] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;
				
				case DEST_ALL_OSC_FO_RATIO:
					m_dDestinations[DEST_OSC1_FO_RATIO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC2_FO_RATIO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC3_FO_RATIO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC4_FO_RATIO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_OSC_OUTPUT_AMP:
					m_dDestinations[DEST_OSC1_OUTPUT_AMP] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC2_OUTPUT_AMP] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC3_OUTPUT_AMP] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_OSC4_OUTPUT_AMP] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_LFO_FO:
					m_dDestinations[DEST_LFO1_FO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_LFO2_FO] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_LFO_OUTPUT_AMP:
					m_dDestinations[DEST_LFO1_OUTPUT_AMP] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_LFO2_OUTPUT_AMP] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_FILTER_FC:
					m_dDestinations[DEST_FILTER1_FC] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_FILTER2_FC] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_FILTER_KEYTRACK:
					m_dDestinations[DEST_FILTER1_KEYTRACK] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_FILTER2_KEYTRACK] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_EG_ATTACK_SCALING:
					m_dDestinations[DEST_EG1_ATTACK_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_EG2_ATTACK_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_EG3_ATTACK_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_EG4_ATTACK_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				case DEST_ALL_EG_DECAY_SCALING:
					m_dDestinations[DEST_EG1_DECAY_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_EG2_DECAY_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_EG3_DECAY_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					m_dDestinations[DEST_EG4_DECAY_SCALING] += dSource*(*p->pModIntensity)*(*p->pModRange);
					break;

				// for all "single" source/dest, this is the modulation
				default:
					m_dDestinations[p->uDestinationIndex] += dSource*(*p->pModIntensity)*(*p->pModRange);
			}
		}
	}

	inline void addModMatrixRow(modMatrixRow* pRow)
	{
		// add if not already existing
		if(!matrixRowExists(pRow->uSourceIndex, pRow->uDestinationIndex))
		{
			// clear register for de-zippering MIDI controls
			pRow->bSmooth = false;
			pRow->dZ1Register = 0.0;

			m_ModMatrix.push_back(pRow);
			m_nSize++;
		}
	}

	// thie deletes the pointer too!
	inline void deleteModMatrix()
	{
		m_MatrixIterator = m_ModMatrix.begin();
		for(int i=0; i<m_nSize; i++)
		{
			modMatrixRow* p = m_ModMatrix[i];

			// remove from list
			m_MatrixIterator = m_ModMatrix.erase(m_MatrixIterator);

			// delete pointer
			delete p;

			m_nSize--;
		}

		m_nSize = 0;
	}

	// thie deletes the pointer too!
	inline void removeMatrixRow(UINT uSourceIndex, UINT uDestinationIndex)
	{
		m_MatrixIterator = m_ModMatrix.begin();
		for(int i=0; i<m_nSize; i++)
		{
			modMatrixRow* p = m_ModMatrix[i];
			
			// find matching source/destination pairs
			if(p->uSourceIndex == uSourceIndex && p->uDestinationIndex == uDestinationIndex)
			{	
				// remove from list
				m_MatrixIterator = m_ModMatrix.erase(m_MatrixIterator);

				// delete pointer
				delete p;

				m_nSize--;
				break;
			}

			m_MatrixIterator++;
		}
	}

	// check for duplicate row
	inline bool matrixRowExists(UINT uSourceIndex, UINT uDestinationIndex)
	{
		for(int i=0; i<m_nSize; i++)
		{
			modMatrixRow* p = m_ModMatrix[i];
			
			// find matching source/destination pairs
			if(p->uSourceIndex == uSourceIndex && p->uDestinationIndex == uDestinationIndex)
			{	
				return true;
			}
		}

		return false;
	}

	// config changes
	inline void clearSources()
	{
		for(int i=0; i<MAX_SOURCES; i++)
		{
			m_dSources[i] = 0.0;
		}
	}

	// called on each pass through the matrix
	inline void clearDestinations()
	{
		for(int i=0; i<MAX_DESTINATIONS; i++)
		{
			m_dDestinations[i] = 0.0;
		}
	}

	// clear in prep for each new modulatin cycle
	inline void setControlSmoother(UINT uSourceIndex)
	{
		for(int i=0; i<m_nSize; i++)
		{
			modMatrixRow* p = m_ModMatrix[i];
			if(p->uSourceIndex == uSourceIndex)
				p->bSmooth = true;
		}	
	}

	// clear out (new note event)
	inline void clearControlSmoothers()
	{
		for(int i=0; i<m_nSize; i++)
		{
			modMatrixRow* p = m_ModMatrix[i];
			p->bSmooth = false;
		}	
	}
};
