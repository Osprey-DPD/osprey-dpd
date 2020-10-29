// FibrilBuilder.h: interface for the CFibrilBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FIBRILBUILDER_H__06f8db5d_f0aa_404a_9ab9_affcdc5afafe__INCLUDED_)
#define AFX_FIBRILBUILDER_H__06f8db5d_f0aa_404a_9ab9_affcdc5afafe__INCLUDED_



#include "Builder.h"

class CFibrilBuilder : private CBuilder
{
	// ****************************************
	// Construction/Destruction:
public:

	CFibrilBuilder(const zLongVector& polymerTypes,
                    long coreBeadType, long linkerBeadType,
					long xn, long yn, long zn, 
					double xc, double yc, double zc, double halfLength, 
					double coreRadius, double linkerRadius,
                    double pitch, double pitchAngle);

	virtual ~CFibrilBuilder();

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

	const zLongVector&				m_PolymerTypes;			// Types of polymer composing fibril

    long        m_CoreBeadType;     // Numeric id of core bead
    long        m_LinkerBeadType;   // Numeric id of linker bead
	long		m_XN;				// Fibril long axis normal: hardwired to 0 0 1
	long		m_YN;
	long		m_ZN;
	double		m_XC;				// Fibril long axis centre as fraction of SimBox thickness: hardwired to 1/2 1/2 1/2
	double		m_YC;
	double		m_ZC;
    double      m_HalfLength;       // Fibril half length: hardwired to 0.5 so fibril fills Z axis
    double      m_CoreRadius;       // Fibril core radius in units of bead diameter
    double      m_LinkerRadius;     // Fibril core radius in units of bead diameter
    double      m_Pitch;            // Monomer thickness in fibril in units of bead diameter
    double      m_PitchAngle;       // Pitch angle of monomers in fibril (radians)

	// Local data

	double m_XCentre;				// Coordinates of fibril long centre
	double m_YCentre;
	double m_ZCentre;

};

#endif // !defined(AFX_FIBRILBUILDER_H__06f8db5d_f0aa_404a_9ab9_affcdc5afafe__INCLUDED_)
