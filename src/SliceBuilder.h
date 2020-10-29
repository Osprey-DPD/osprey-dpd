// SliceBuilder.h: interface for the CSliceBuilder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLICEBUILDER_H__E8F3C82F_A867_4C83_B4C7_A856C1923765__INCLUDED_)
#define AFX_SLICEBUILDER_H__E8F3C82F_A867_4C83_B4C7_A856C1923765__INCLUDED_



#include "Builder.h"

class CSliceBuilder : private CBuilder  
{
	// ****************************************
	// Construction/Destruction:
public:

	CSliceBuilder(long slabTotal, long xn, long yn, long zn, zDoubleVector vWidths, 
                  LongStringMMap mmPolymers, StringLongMap mPolymerTypes);

	virtual ~CSliceBuilder();

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

    // Local data

    double  m_SimBoxWidth;            // Length of SimBox in direction of slab normal
    double  m_WallWidth;              // Wall width in direction of slab normal (0 if no wall)
};

#endif // !defined(AFX_SLICEBUILDER_H__E8F3C82F_A867_4C83_B4C7_A856C1923765__INCLUDED_)
