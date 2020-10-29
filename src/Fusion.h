// Fusion.h: interface for the CFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUSION_H__88FC9133_4101_11D4_BF3D_004095086186__INCLUDED_)
#define AFX_FUSION_H__88FC9133_4101_11D4_BF3D_004095086186__INCLUDED_


// Forward declarations

class CFusionData;


#include "Analysis.h"

class CFusion : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CFusion(const zString runId, long sample, const CFusionData* const pAD);
	virtual ~CFusion();

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

#endif // !defined(AFX_FUSION_H__88FC9133_4101_11D4_BF3D_004095086186__INCLUDED_)

