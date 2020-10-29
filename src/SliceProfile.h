// SliceProfile.h: interface for the CSliceProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SLICEPROFILE_H__C2278A17_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_SLICEPROFILE_H__C2278A17_AC86_11D3_BF15_004095086186__INCLUDED_



// Forward declarations

class ISimBox;

#include "Profile.h"
#include "Slice.h"


class CSliceProfile : public CProfile<CSlice>  
{
public:
	CSliceProfile();
	CSliceProfile(long sliceTotal, long xNormal, long yNormal, long zNormal, 
		          const ISimBox* const pISimBox);

	virtual ~CSliceProfile();

	inline long GetXNormal()  {return GetRegion(0)->GetXNormal();}
	inline long GetYNormal()  {return GetRegion(0)->GetYNormal();}
	inline long GetZNormal()  {return GetRegion(0)->GetZNormal();}
};

#endif // !defined(AFX_SLICEPROFILE_H__C2278A17_AC86_11D3_BF15_004095086186__INCLUDED_)
