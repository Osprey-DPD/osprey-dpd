// MicelleBuilder.h: interface for the CMicelleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICELLEBUILDER_H__E1A859B4_87BD_11D4_BF49_004095086186__INCLUDED_)
#define AFX_MICELLEBUILDER_H__E1A859B4_87BD_11D4_BF49_004095086186__INCLUDED_


#include "Builder.h"

class CMicelleBuilder : private CBuilder  
{
public:
	CMicelleBuilder(zLongVector polymerTypes, double xc, double yc, double zc,  
					double radius);
	virtual ~CMicelleBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	zLongVector m_PolymerTypes;
	double m_XC;
	double m_YC;
	double m_ZC;
	double m_Radius;
};

#endif // !defined(AFX_MICELLEBUILDER_H__E1A859B4_87BD_11D4_BF49_004095086186__INCLUDED_)
