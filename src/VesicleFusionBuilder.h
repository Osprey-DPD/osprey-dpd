// VesicleFusionBuilder.h: interface for the CVesicleFusionBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VESICLEFUSIONBUILDER_H__E422893D_B0D2_4069_AF10_A1ACDBBADDE3__INCLUDED_)
#define AFX_VESICLEFUSIONBUILDER_H__E422893D_B0D2_4069_AF10_A1ACDBBADDE3__INCLUDED_


#include "Builder.h"

class CVesicleFusionBuilder : private CBuilder  
{
public:
	CVesicleFusionBuilder(zLongVector polymerTypes1, zLongVector polymerTypes2, 
						  zDoubleVector outerFractions1, zDoubleVector outerFractions2,
						  const double xc[2], const double yc[2], const double zc[2],  
						  const double outerRadius[2], const double innerRadius[2], 
						  const double thickness[2], 
						  const bool bPatches1[2], const bool bPatches2[2]);

	virtual ~CVesicleFusionBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	zLongVector		m_PolymerTypes1;	// Types of polymer composing vesicle 1
	zLongVector		m_PolymerTypes2;	// Types of polymer composing vesicle 2
	zDoubleVector	m_OuterFractions1;	// Fractions of polymers in outer monolayer
	zDoubleVector	m_OuterFractions2;	// Fractions of polymers in outer monolayer
	double			m_XC[2];			// Vesicle centres as fraction of SimBox thickness
	double			m_YC[2];
	double			m_ZC[2];
	double			m_OuterRadius[2];	// Vesicle outer monolayer head radii in units of bead diameters
	double			m_InnerRadius[2];	// Vesicle inner monolayer head radii in units of bead diameters
	double			m_Thickness[2];		// Vesicle bilayer thicknesses in units of bead radius
	bool			m_bPatches1[2];		// Distribute polymers homogeneously (0) or in patches (1)
	bool			m_bPatches2[2];		// Distribute polymers homogeneously (0) or in patches (1)
};

#endif // !defined(AFX_VESICLEFUSIONBUILDER_H__E422893D_B0D2_4069_AF10_A1ACDBBADDE3__INCLUDED_)
