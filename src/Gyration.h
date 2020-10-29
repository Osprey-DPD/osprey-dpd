// Gyration.h: Gyration for the CGyration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GYRATION_H__064160d6_b6e2_4ed4_b87a_a7d72573af99__INCLUDED_)
#define AFX_GYRATION_H__064160d6_b6e2_4ed4_b87a_a7d72573af99__INCLUDED_


// Forward declarations

class CGyrationData;

// Include files

#include "Analysis.h"

class CGyration : public CAnalysis
{
	// ****************************************
	// Construction/Destruction
public:

	CGyration(const zString runId, long sample, const CGyrationData* const pAD);
	virtual ~CGyration();

	// ****************************************
	// IEventXXX interface functions for all supported events
public:


	// ****************************************
	// Public access functions
public:

	
	// ****************************************
	// PVS inherited from CAnalysis base class
public:

	// Function to get information about the analysis type from the ISimBox

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);

	// Function to convert any bead, bond or polymer names from strings to integers

	virtual void ConvertNames(const CAnalysisState& raState);

private:

    
private:						

    StringSequence	m_Polymers;			// Names of polymer types to use

    bool m_bByType;                     // Flag showing whether to output per polymer type as well

};

#endif // !defined(AFX_GYRATION_H__064160d6_b6e2_4ed4_b87a_a7d72573af99__INCLUDED_)
