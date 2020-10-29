// AnalysisTool.h: interface for the CAnalysisTool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALYSISTOOL_H__04F412D0_1A8E_4DA1_9969_0F88F2E57B41__INCLUDED_)
#define AFX_ANALYSISTOOL_H__04F412D0_1A8E_4DA1_9969_0F88F2E57B41__INCLUDED_


// Include files to allow all derived classes to access the SimBox data.

#include "SimState.h"
#include "ISimBox.h"


#include "xxBase.h"

class CAnalysisTool  : public xxBase
{
public:
	virtual ~CAnalysisTool();

	// PVF required in all derived classes to perform the analysis tool's
	// particular calculations of aggregate properties.

	virtual void UpdateState(CSimState& rSimState, const ISimBox* const pISimBox) = 0;


	// Protected constructor ensure that only derived classes can be created.

protected:
	CAnalysisTool();

};

#endif // !defined(AFX_ANALYSISTOOL_H__04F412D0_1A8E_4DA1_9969_0F88F2E57B41__INCLUDED_)
