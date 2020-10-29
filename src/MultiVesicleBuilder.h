// MultiVesicleBuilder.h: interface for the CMultiVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIVESICLEBUILDER_H__2CB65C38_BB2C_47B3_A7C9_75C90BCC2E1D__INCLUDED_)
#define AFX_MULTIVESICLEBUILDER_H__2CB65C38_BB2C_47B3_A7C9_75C90BCC2E1D__INCLUDED_


#include "Builder.h"

class CMultiVesicleBuilder : private CBuilder  
{
public:
	CMultiVesicleBuilder(zLongVector polymerTypes, double xc, double yc, double zc,  
						 double outerRadius, double innerRadius, double thickness);

	virtual ~CMultiVesicleBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	zLongVector	m_PolymerTypes;		// Types of polymer composing micelle
	double		m_XC;				// Vesicle centre as fraction of SimBox thickness
	double		m_YC;
	double		m_ZC;
	double		m_OuterRadius;		// Vesicle outer monolayer head radius in units of bead diameters
	double		m_InnerRadius;		// Vesicle inner monolayer head radius in units of bead diameters
	double		m_Thickness;		// Vesicle bilayer thickness in units of bead radius

};

#endif // !defined(AFX_MULTIVESICLEBUILDER_H__2CB65C38_BB2C_47B3_A7C9_75C90BCC2E1D__INCLUDED_)
