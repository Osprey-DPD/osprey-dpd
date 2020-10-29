// InitialStateSlice.h: interface for the CInitialStateSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATESLICE_H__3AD9B3D7_13DA_4F73_BDD3_3AE03F9DBB54__INCLUDED_)
#define AFX_INITIALSTATESLICE_H__3AD9B3D7_13DA_4F73_BDD3_3AE03F9DBB54__INCLUDED_



#include "InitialStateData.h"

class CInitialStateSlice : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction:
public:

	CInitialStateSlice();

	CInitialStateSlice& operator=(const CInitialStateSlice& oldIS);

	virtual ~CInitialStateSlice();

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

    long            m_SlabTotal;        // Number of slabs
	long			m_XN;				// Slab normal
	long			m_YN;
	long			m_ZN;

    zDoubleVector   m_vWidths;          // Slab widths as fraction of SimBox thickness

    LongStringMMap  m_mmPolymers;       // Multimap of (slab number, polymer name) pairs

    // Local data

    StringLongMap   m_mPolymerTypes;    // Map of (polymer name, polymer type) pairs

};

#endif // !defined(AFX_INITIALSTATESLICE_H__3AD9B3D7_13DA_4F73_BDD3_3AE03F9DBB54__INCLUDED_)
