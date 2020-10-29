// CellProfileSet.h: interface for the CCellProfileSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELLPROFILESET_H__303A22CA_C74D_11D3_BF1C_004095086186__INCLUDED_)
#define AFX_CELLPROFILESET_H__303A22CA_C74D_11D3_BF1C_004095086186__INCLUDED_


// Forward declarations

class CRowProfile;


#include "Profile.h"
#include "CellProfile.h"

class CCellProfileSet  : public CProfile<CCellProfile>
{
public:
	CCellProfileSet(CRowProfile* const pRP, long cellTotal);
	virtual ~CCellProfileSet();

	CCellProfile* GetProfile(long index);
	CCellProfile* GetOrthogonalProfile(long index) const;
	inline long GetOrthogonalSize() const {return m_vOrthogonalSet.size();}


private:
	CellProfileVector  m_vOrthogonalSet;	// Set of orthogonal cell profiles

};

#endif // !defined(AFX_CELLPROFILESET_H__303A22CA_C74D_11D3_BF1C_004095086186__INCLUDED_)
