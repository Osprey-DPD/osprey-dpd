// Adhesion.h: interface for the CAdhesion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADHESION_H__88FC9135_4101_11D4_BF3D_004095086186__INCLUDED_)
#define AFX_ADHESION_H__88FC9135_4101_11D4_BF3D_004095086186__INCLUDED_


// Forward declarations

class CAdhesionData;


#include "Analysis.h"

class CAdhesion : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CAdhesion(const zString runId, long sample, const CAdhesionData* const pAD);
	virtual ~CAdhesion();


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

#endif // !defined(AFX_ADHESION_H__88FC9135_4101_11D4_BF3D_004095086186__INCLUDED_)
