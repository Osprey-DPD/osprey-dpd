// RestartBuilder.h: interface for the CRestartBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESTARTBUILDER_H__BAEB2C00_44FF_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_RESTARTBUILDER_H__BAEB2C00_44FF_11D3_820E_0060088AD300__INCLUDED_


#include "Builder.h"

class CRestartBuilder : private CBuilder  
{
public:
	CRestartBuilder(zString runId, long stateId);
	virtual ~CRestartBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	zString m_RunId;		// Read from user may be letters or numbers
	long m_StateId;		// Taken from current time when restart state was saved

};

#endif // !defined(AFX_RESTARTBUILDER_H__BAEB2C00_44FF_11D3_820E_0060088AD300__INCLUDED_)
