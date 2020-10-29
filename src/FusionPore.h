// FusionPore.h: interface for the CFusionPore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FUSIONPORE_H__E8DD1925_C12B_11D4_BF58_004095086186__INCLUDED_)
#define AFX_FUSIONPORE_H__E8DD1925_C12B_11D4_BF58_004095086186__INCLUDED_


// Forward declarations

class CFusionPoreData;

#include "Analysis.h"

class CFusionPore : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CFusionPore(const zString runId, long sample, const CFusionPoreData* const pAD);
	virtual ~CFusionPore();

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

#endif // !defined(AFX_FUSIONPORE_H__E8DD1925_C12B_11D4_BF58_004095086186__INCLUDED_)
