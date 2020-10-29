// BondRupture.h: interface for the CBondRupture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BONDRUPTURE_H__88FC9134_4101_11D4_BF3D_004095086186__INCLUDED_)
#define AFX_BONDRUPTURE_H__88FC9134_4101_11D4_BF3D_004095086186__INCLUDED_


// Forward declarations

class CBondRuptureData;


#include "Analysis.h"

class CBondRupture : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CBondRupture(const zString runId, long sample, const CBondRuptureData* const pAD);
	virtual ~CBondRupture();

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

#endif // !defined(AFX_BONDRUPTURE_H__88FC9134_4101_11D4_BF3D_004095086186__INCLUDED_)
