// InitialStateFibril.h: interface for the CInitialStateFibril class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEFIBRIL_H__36ec1094_69f1_4876_b476_4e60956b60ea__INCLUDED_)
#define AFX_INITIALSTATEFIBRIL_H__36ec1094_69f1_4876_b476_4e60956b60ea__INCLUDED_



#include "InitialStateData.h"

class CInitialStateFibril : public CInitialStateData
{
	// ****************************************
	// Construction/Destruction:
public:

	CInitialStateFibril();

	CInitialStateFibril& operator=(const CInitialStateFibril& oldISV);

	virtual ~CInitialStateFibril();

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

    StringSequence  m_Polymers;         // Polymers composing fibril
    zString         m_CoreBead;         // Name of core bead
    zString         m_LinkerBead;       // Name of linker bead
	long			m_XN;				// Long axis normal
	long			m_YN;
	long			m_ZN;
	double			m_XC;				// Long axis centre as fraction of SimBox thickness: hardwired to centre
	double			m_YC;
	double			m_ZC;
    double          m_HalfLength;      // Fibril half length hardwired to 0.5 as fibril is frozen
    double          m_CoreRadius;      // Fibril core radius in units of bead diameter
    double          m_LinkerRadius;    // Fibril linker length in units of bead diameter
    double          m_Pitch;           // Monomer thickness in fibril
    double          m_PitchAngle;      // Pitch angle along fibril between adjacent monomers


	// Local data

	zLongVector   m_PolymerTypes;			// Types of polymer composing fibril
    long          m_CoreBeadType;           // Numeric type of core beads
    long          m_LinkerBeadType;         // Numeric type of linker beads

};

#endif // !defined(AFX_INITIALSTATEFIBRIL_H__36ec1094_69f1_4876_b476_4e60956b60ea__INCLUDED_)
