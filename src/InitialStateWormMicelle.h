// InitialStateWormMicelle.h: interface for the CInitialStateWormMicelle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEWORMMICELLE_H__BA5269E6_3014_44A8_BA4D_076E073FC529__INCLUDED_)
#define AFX_INITIALSTATEWORMMICELLE_H__BA5269E6_3014_44A8_BA4D_076E073FC529__INCLUDED_



#include "InitialStateData.h"

class CInitialStateWormMicelle : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction:
public:

	CInitialStateWormMicelle();

	CInitialStateWormMicelle& operator=(const CInitialStateWormMicelle& oldISV);

	virtual ~CInitialStateWormMicelle();

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

private:

	StringSequence	m_Polymers;			// Polymers composing worm
	long			m_XN;				// Worm axis normal
	long			m_YN;
	long			m_ZN;
	double			m_XC;				// Worm axis centre as fraction of SimBox thickness
	double			m_YC;
	double			m_ZC;
	double			m_Radius;			// Worm radius in units of bead diameter
	double			m_HalfLength;		// Worm half length as fraction of SimBox thickness
	bool			m_bPatches;			// Arrange polymers in patches: 0/1

	// Data relating to a polymerised initial state
	bool					m_bPolymerise;	// Flag showing if initial state is polymerised
	PolymerCrossLinkVector	m_CrossLinks;	// Vector of cross-linker pointers holding data

	// Local data

	zLongVector   m_PolymerTypes;			// Types of polymer composing vesicle

};

#endif // !defined(AFX_INITIALSTATEWORMMICELLE_H__BA5269E6_3014_44A8_BA4D_076E073FC529__INCLUDED_)
