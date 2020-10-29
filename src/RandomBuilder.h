// RandomBuilder.h: interface for the CRandomBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANDOMBUILDER_H__A2FACF43_3F61_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_RANDOMBUILDER_H__A2FACF43_3F61_11D3_820E_0060088AD300__INCLUDED_


#include "Builder.h"

class CRandomBuilder : private CBuilder  
{
public:

	CRandomBuilder();
	virtual ~CRandomBuilder();

	virtual bool Assemble(CInitialState& riState);
	virtual bool AssembleP(CInitialState& riState);

};

#endif // !defined(AFX_RANDOMBUILDER_H__A2FACF43_3F61_11D3_820E_0060088AD300__INCLUDED_)
