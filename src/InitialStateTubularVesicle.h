// InitialStateTubularVesicle.h: interface for the CInitialStateTubularVesicle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATETUBULARVESICLE_H__7ABE7EE0_00E3_4377_81C5_DB6580565339__INCLUDED_)
#define AFX_INITIALSTATETUBULARVESICLE_H__7ABE7EE0_00E3_4377_81C5_DB6580565339__INCLUDED_


#include "InitialStateData.h"

class CInitialStateTubularVesicle : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction:
public:

	CInitialStateTubularVesicle();

	CInitialStateTubularVesicle& operator=(const CInitialStateTubularVesicle& oldISV);

	virtual ~CInitialStateTubularVesicle();

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
	virtual bool Assemble(CInitialState& riState) const;
	virtual CInitialStateData* Copy() const;

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

	StringSequence	m_Polymers;		// Polymers composing vesicle
	StringSequence	m_InteriorPolymers;	// Polymers in interior of vesicle
	double		m_XC;			// Vesicle centre as fraction of SimBox thickness
	double		m_YC;
	double		m_ZC;
	double		m_OuterRadius;		// Vesicle outer monolayer head radius in units of bead diameter
	double		m_Thickness;		// Vesicle bilayer thickness in units of bead diameter
	zDoubleVector	m_OuterFractions;	// Fractions of polymers in outer monolayer
	bool		m_bPatches[2];		// Arrange polymers in patches: 0/1

	// Data relating to a polymerised initial state
	bool		m_bPolymerise;	// Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_CrossLinks;	// Vector of cross-linker pointers holding data

	// Local data

	zLongVector   m_PolymerTypes;		// Types of polymer composing vesicle
	zLongVector   m_InteriorPolymerTypes;	// Types of polymer inside vesicle

	double	m_InnerRadius;				// Vesicle inner monolayer head radius in units of bead diameter


};

#endif // !defined(AFX_INITIALSTATETUBULARVESICLE_H__7ABE7EE0_00E3_4377_81C5_DB6580565339__INCLUDED_)
