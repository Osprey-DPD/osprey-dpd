// Pores.h: interface for the CPores class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PORES_H__88FC9136_4101_11D4_BF3D_004095086186__INCLUDED_)
#define AFX_PORES_H__88FC9136_4101_11D4_BF3D_004095086186__INCLUDED_


// Forward declarations

class CPoresData;


#include "Analysis.h"

class CPores : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CPores(const zString runId, long sample, const CPoresData* const pAD);
	virtual ~CPores();

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

#endif // !defined(AFX_PORES_H__88FC9136_4101_11D4_BF3D_004095086186__INCLUDED_)
