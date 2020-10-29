// ColumnBuilder.h: interface for the CColumnBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COLUMNBUILDER_H__BF4EC603_0125_4099_922C_EB816954527F__INCLUDED_)
#define AFX_COLUMNBUILDER_H__BF4EC603_0125_4099_922C_EB816954527F__INCLUDED_



#include "Builder.h"

class CColumnBuilder : private CBuilder  
{
	// ****************************************
	// Construction/Destruction:
public:

	CColumnBuilder(long slabTotal, long xn, long yn, long zn, zDoubleVector vWidths, 
                  LongStringMMap mmPolymers, StringLongMap mPolymerTypes);

	virtual ~CColumnBuilder();

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


    long        m_SlabTotal;        // Number of slabs
	long		m_XN;				// Slice normal
	long		m_YN;
	long		m_ZN;

    zDoubleVector   m_vWidths;        // Slab widths as fraction of SimBox thickness
    LongStringMMap  m_mmPolymers;     // Multimap of (slab number, polymer name) pairs
    StringLongMap   m_mPolymerTypes;  // Map of (polymer name, polymer type ) pairs

};

#endif // !defined(AFX_COLUMNBUILDER_H__BF4EC603_0125_4099_922C_EB816954527F__INCLUDED_)
