// TubularVesicleBuilder.h: interface for the CTubularVesicleBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TUBULARVESICLEBUILDER_H__EE942BE8_F281_4420_9F6C_1D8ADA06B836__INCLUDED_)
#define AFX_TUBULARVESICLEBUILDER_H__EE942BE8_F281_4420_9F6C_1D8ADA06B836__INCLUDED_



#include "Builder.h"

class CTubularVesicleBuilder : private CBuilder  
{
	// ****************************************
	// Construction/Destruction:
public:

	CTubularVesicleBuilder(const zLongVector& polymerTypes, 
					const zLongVector& interiorPolymerTypes, 
					const zDoubleVector& outerFractions, 
					const PolymerCrossLinkVector& crossLinks,
					double xc, double yc, double zc, 
					double outerRadius, double innerRadius, double thickness, 
					const bool bPatches[2], bool bPolymerise);

	virtual ~CTubularVesicleBuilder();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	virtual bool Assemble(CInitialState& riState);

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members
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

#endif // !defined(AFX_TUBULARVESICLEBUILDER_H__EE942BE8_F281_4420_9F6C_1D8ADA06B836__INCLUDED_)

