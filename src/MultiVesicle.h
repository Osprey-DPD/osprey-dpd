// MultiVesicle.h: interface for the CMultiVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIVESICLE_H__5506CDFE_D9ED_431C_9733_2030856F8B79__INCLUDED_)
#define AFX_MULTIVESICLE_H__5506CDFE_D9ED_431C_9733_2030856F8B79__INCLUDED_


// Forward declarations

class CMultiVesicleData;



#include "Analysis.h"

class CMultiVesicle : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CMultiVesicle(const zString runId, long sample, const CMultiVesicleData* const pAD);
	virtual ~CMultiVesicle();

	// ****************************************
	// IEventXXX interface functions for all supported events
public:


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// PVS inherited from CAnalysis base class
public:
	virtual void ConvertNames(const CAnalysisState& raState);
	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox);


};

#endif // !defined(AFX_MULTIVESICLE_H__5506CDFE_D9ED_431C_9733_2030856F8B79__INCLUDED_)
