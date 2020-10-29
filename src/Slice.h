// Slice.h: interface for the CSlice class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLICE_H__C2278A1B_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_SLICE_H__C2278A1B_AC86_11D3_BF15_004095086186__INCLUDED_



// Forward declarations

class ISimBox;

#include "Region.h"

class CSlice : public CRegion  
{
public:

	// Constructor for use when grid slice width < or > CNT cell width

	CSlice(long sliceIndex, long lowerCNTSliceIndex, long upperCNTSliceIndex,
		   double lowerSliceCoord, double upperSliceCoord, 
		   long xNormal, long yNormal, long zNormal, 
		   const ISimBox* const pISimBox);

	// Constructor for use when grid slice width = CNT cell width

	CSlice(long sliceIndex, long xNormal, long yNormal, long zNormal, 
		   const ISimBox* const pISimBox);

	// Alternative constructor used in force target commands

	CSlice(long xCoord, long yCoord, long zCoord, 
		   const ISimBox* const pISimBox);

	CSlice(const CSlice& oldSlice);

	virtual ~CSlice();

	inline long GetXNormal()		const {return m_X;}
	inline long GetYNormal()		const {return m_Y;}
	inline long GetZNormal()		const {return m_Z;}
	inline long GetCNTRowsInSlice() const {return m_CNTRowsInSlice;}

private:
	long m_X;				// Normal defining slice orientation
	long m_Y;
	long m_Z;
	long m_CNTRowsInSlice;	// Number of CNT cells in the depth of the slice
};

#endif // !defined(AFX_SLICE_H__C2278A1B_AC86_11D3_BF15_004095086186__INCLUDED_)
