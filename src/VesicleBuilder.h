// VesicleBuilder.h: interface for the CVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VESICLEBUILDER_H__EC62FB35_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_VESICLEBUILDER_H__EC62FB35_3C4F_11D4_BF3C_004095086186__INCLUDED_


#include "Builder.h"

class CVesicleBuilder : private CBuilder  
{
public:
	CVesicleBuilder(const zLongVector& polymerTypes, 
					const zLongVector& interiorPolymerTypes, 
					const zDoubleVector& outerFractions, 
					const PolymerCrossLinkVector& crossLinks,
					double xc, double yc, double zc, 
					double outerRadius, double innerRadius, double thickness, 
					const bool bPatches[2], bool bPolymerise);

	virtual ~CVesicleBuilder();

	virtual bool Assemble(CInitialState& riState);

private:
	const zLongVector&				m_PolymerTypes;			// Types of polymer composing vesicle
	const zLongVector&				m_InteriorPolymerTypes;	// Types of polymer in vesicle interior
	const zDoubleVector&			m_OuterFractions;		// Fractions of polymers in outer monolayer
	const PolymerCrossLinkVector&	m_CrossLinks;			// Polymerisation data 
	double		m_XC;				// Vesicle centre as fraction of SimBox thickness
	double		m_YC;
	double		m_ZC;
	double		m_OuterRadius;		// Vesicle outer monolayer head radius in units of bead diameters
	double		m_InnerRadius;		// Vesicle inner monolayer head radius in units of bead diameters
	double		m_Thickness;		// Vesicle bilayer thickness in units of bead radius
	bool		m_bPatches[2];		// Distribute polymers randomly (0) or in patches (1)
	bool		m_bPolymerise;		// Flag showing if vesicle is polymerised

	// Local data

	double m_XCentre;				// Coordinates of vesicle centre
	double m_YCentre;
	double m_ZCentre;

};

#endif // !defined(AFX_VESICLEBUILDER_H__EC62FB35_3C4F_11D4_BF3C_004095086186__INCLUDED_)
