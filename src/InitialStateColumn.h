// InitialStateColumn.h: interface for the CInitialStateColumn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATECOLUMN_H__2F37815C_9B76_447B_A9B0_55E6090EE80E__INCLUDED_)
#define AFX_INITIALSTATECOLUMN_H__2F37815C_9B76_447B_A9B0_55E6090EE80E__INCLUDED_



#include "InitialStateData.h"

class CInitialStateColumn : public CInitialStateData  
{
	// ****************************************
	// Construction/Destruction:
public:

	CInitialStateColumn();

	CInitialStateColumn& operator=(const CInitialStateColumn& oldIS);

	virtual ~CInitialStateColumn();

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

#endif // !defined(AFX_INITIALSTATECOLUMN_H__2F37815C_9B76_447B_A9B0_55E6090EE80E__INCLUDED_)
