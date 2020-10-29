// RowProfile.h: interface for the CRowProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROWPROFILE_H__C2278A18_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_ROWPROFILE_H__C2278A18_AC86_11D3_BF15_004095086186__INCLUDED_


#include "Slice.h"

#include "Profile.h"
#include "Row.h"

class CRowProfile : public CProfile<CRow>  
{
public:
	CRowProfile(CSlice* const pSlice, long rowTotal);	// Constructor using a CSlice object
	CRowProfile(RowVector vRows);				// Constructor using a set of CRows

	virtual ~CRowProfile();

	inline long GetRowXNormal()			  {return GetRegion(0)->GetXNormal();}	// Row normal
	inline long GetRowYNormal()			  {return GetRegion(0)->GetYNormal();}
	inline long GetRowZNormal()			  {return GetRegion(0)->GetZNormal();}
	inline long GetProfileXNormal() const {return m_pSlice->GetXNormal();}		// Slice normal
	inline long GetProfileYNormal() const {return m_pSlice->GetYNormal();}
	inline long GetProfileZNormal() const {return m_pSlice->GetZNormal();}

private:
	CSlice* const m_pSlice;		// Pointer is const not the CSlice
};

#endif // !defined(AFX_ROWPROFILE_H__C2278A18_AC86_11D3_BF15_004095086186__INCLUDED_)
