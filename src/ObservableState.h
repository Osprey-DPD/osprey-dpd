// ObservableState.h: interface for the CObservableState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBSERVABLESTATE_H__A6EF66C0_61F7_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_OBSERVABLESTATE_H__A6EF66C0_61F7_11D3_820E_0060088AD300__INCLUDED_


#include "xxState.h"

// Forward declarations

class CInitialState;

class CObservableState : public xxState  
{
public:
	CObservableState(const long currentTime, const zString runId, const CInitialState& riState);
	virtual ~CObservableState();

};

#endif // !defined(AFX_OBSERVABLESTATE_H__A6EF66C0_61F7_11D3_820E_0060088AD300__INCLUDED_)
