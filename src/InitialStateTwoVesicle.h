// InitialStateTwoVesicle.h: interface for the CInitialStateTwoVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATETWOVESICLE_H__8C2A0D27_F5EC_4A4A_A0A5_70C102D8FDAB__INCLUDED_)
#define AFX_INITIALSTATETWOVESICLE_H__8C2A0D27_F5EC_4A4A_A0A5_70C102D8FDAB__INCLUDED_



#include "InitialStateData.h"

class CInitialStateTwoVesicle : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction
public:

	CInitialStateTwoVesicle();

	CInitialStateTwoVesicle& operator=(const CInitialStateTwoVesicle& oldISV);

	virtual ~CInitialStateTwoVesicle();

	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();

private:

	static const zString m_Type;		// Keyword used to identify initial state type


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	zOutStream& put(zOutStream& os) const;		// overrides the virtual base function
	zInStream&  get(zInStream&  is);			// overrides the virtual base function

	virtual zString GetInitialStateType() const;
	virtual CInitialStateData* Copy() const;

	virtual bool Assemble(CInitialState& riState) const;

	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	virtual bool ValidateData(const CInputData& riData);

	// ****************************************
	// Implementation

	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

	StringSequence m_Polymers1;			// Polymers composing first vesicle
	StringSequence m_Polymers2;			// Polymers composing second vesicle
	StringSequence m_Interior1;			// Polymers inside first vesicle
	StringSequence m_Interior2;			// Polymers inside second vesicle
	double	m_XC[2];
	double	m_YC[2];					// Vesicle centres as fractions of SimBox thickness
	double	m_ZC[2];
	double	m_OuterRadius[2];			// Vesicle outer monolayer head radii in units of bead diameter
	double	m_Thickness[2];				// Vesicle bilayer thicknesses in units of bead diameter
	zDoubleVector	m_OuterFractions1;	// Fractions of polymers in outer monolayer
	zDoubleVector	m_OuterFractions2;	// Fractions of polymers in outer monolayer
	bool	m_bPatches1[2];				// Arrange polymers in patches for first vesicle: 0/1
	bool	m_bPatches2[2];				// Arrange polymers in patches for second vesicle

	// Local data

	zLongVector   m_PolymerTypes1;		// Types of polymer composing first vesicle
	zLongVector   m_PolymerTypes2;		// Types of polymer composing second vesicle
	zLongVector   m_InteriorTypes1;		// Types of polymer inside first vesicle
	zLongVector   m_InteriorTypes2;		// Types of polymer inside second vesicle

	double	m_InnerRadius[2];			// Vesicle inner monolayer head radii in units of bead diameter

};

#endif // !defined(AFX_INITIALSTATETWOVESICLE_H__8C2A0D27_F5EC_4A4A_A0A5_70C102D8FDAB__INCLUDED_)
