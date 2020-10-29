// RowProfileSet.h: interface for the CRowProfileSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROWPROFILESET_H__303A22C9_C74D_11D3_BF1C_004095086186__INCLUDED_)
#define AFX_ROWPROFILESET_H__303A22C9_C74D_11D3_BF1C_004095086186__INCLUDED_


// Forward declarations

class CSliceProfile;
class CCellProfileSet;

#include "Profile.h"
#include "RowProfile.h"

class CRowProfileSet : public CProfile<CRowProfile>
{
public:
	CRowProfileSet(CSliceProfile* const pSP, long rowTotal);
	virtual ~CRowProfileSet();

	CRowProfile* GetProfile(long index);
	CRowProfile* GetOrthogonalProfile(long index) const;
	inline long GetOrthogonalSize() const {return m_vOrthogonalSet.size();}

private:
	RowProfileVector  m_vOrthogonalSet;	// Set of orthogonal row profiles

};

#endif // !defined(AFX_ROWPROFILESET_H__303A22C9_C74D_11D3_BF1C_004095086186__INCLUDED_)
