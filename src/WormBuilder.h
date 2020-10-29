// WormBuilder.h: interface for the CWormBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORMBUILDER_H__056D9F82_D9EF_4BEA_9F0B_28EC85591C1D__INCLUDED_)
#define AFX_WORMBUILDER_H__056D9F82_D9EF_4BEA_9F0B_28EC85591C1D__INCLUDED_



#include "Builder.h"

class CWormBuilder : private CBuilder  
{
	// ****************************************
	// Construction/Destruction:
public:

	CWormBuilder(const zLongVector& polymerTypes, 
					const PolymerCrossLinkVector& crossLinks,
					long xn, long yn, long zn, 
					double xc, double yc, double zc, 
					double radius, double halfLength, 
					bool bPatches, bool bPolymerise);

	virtual ~CWormBuilder();

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
	const PolymerCrossLinkVector&	m_CrossLinks;			// Polymerisation data 

	long		m_XN;				// Worm axis normal
	long		m_YN;
	long		m_ZN;
	double		m_XC;				// Worm axis centre as fraction of SimBox thickness
	double		m_YC;
	double		m_ZC;
	double		m_Radius;			// Worm radius in units of bead diameters
	double		m_HalfLength;		// Worm half length in units of bead radius
	bool		m_bPatches;			// Distribute polymers randomly (0) or in patches (1)
	bool		m_bPolymerise;		// Flag showing if work is polymerised

	// Local data

	double m_XCentre;				// Coordinates of worm axis centre
	double m_YCentre;
	double m_ZCentre;

};

#endif // !defined(AFX_WORMBUILDER_H__056D9F82_D9EF_4BEA_9F0B_28EC85591C1D__INCLUDED_)
