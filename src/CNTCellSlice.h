// CNTCellSlice.h: interface for the CCNTCellSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CNTCELLSLICE_H__EC8F123C_9A6D_40CD_B32B_66705B15AD18__INCLUDED_)
#define AFX_CNTCELLSLICE_H__EC8F123C_9A6D_40CD_B32B_66705B15AD18__INCLUDED_


// Forward declarations

class ISimBox;


// Include file for the contained CCNTCellProfile object used to hold the cells.

#include "CNTCellProfile.h"


class CCNTCellSlice  
{
public:
	CCNTCellSlice(long sliceIndex, long xNormal, long yNormal, long zNormal, 
				  const ISimBox* const pISimBox);
	virtual ~CCNTCellSlice();

	// Access functions to the cells.

	inline long GetIndex()		    const {return m_SliceIndex;}
	inline long GetXNormal()		const {return m_XN;}
	inline long GetYNormal()		const {return m_YN;}
	inline long GetZNormal()		const {return m_ZN;}
	inline long GetCellsPerSlice()  const {return m_CellsPerSlice;}

	long GetSize() const;
	const CCNTCell* GetCell(long index);

private:

	const long m_SliceIndex;	// The index runs from 0 to N-1 in each direction
	const long m_XN;			// Only one component can be 1, other two must be 0
	const long m_YN;			// Only one component can be 1, other two must be 0
	const long m_ZN;			// Only one component can be 1, other two must be 0
	const long m_XCellNo;
	const long m_YCellNo;		// No of CNT cells in each dimension of SimBox
	const long m_ZCellNo;

	long m_CellsPerSlice;

	CCNTCellProfile		m_Cells;
};

#endif // !defined(AFX_CNTCELLSLICE_H__EC8F123C_9A6D_40CD_B32B_66705B15AD18__INCLUDED_)
