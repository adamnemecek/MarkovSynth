#include "ModulationMatrix.h"

CModulationMatrix::CModulationMatrix(void)
{
	m_nSize = 0;
	clearSources();
	clearDestinations();
}

CModulationMatrix::~CModulationMatrix(void)
{
	deleteModMatrix();
}
