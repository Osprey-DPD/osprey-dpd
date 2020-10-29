// VesicleFusion.h: interface for the CVesicleFusion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VESICLEFUSION_H__6F71F620_B702_407A_A3C8_91E4664EF8F2__INCLUDED_)
#define AFX_VESICLEFUSION_H__6F71F620_B702_407A_A3C8_91E4664EF8F2__INCLUDED_


// Forward declarations

class CVesicleFusionData;


#include "Analysis.h"

class CVesicleFusion : public CAnalysis  
{
	// ****************************************
	// Construction/Destruction
public:

	CVesicleFusion(const zString runId, long sample, const CVesicleFusionData* const pAD);
	virtual ~CVesicleFusion();

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

#endif // !defined(AFX_VESICLEFUSION_H__6F71F620_B702_407A_A3C8_91E4664EF8F2__INCLUDED_)

