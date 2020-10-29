// CellProfile.h: interface for the CCellProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELLPROFILE_H__C2278A19_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_CELLPROFILE_H__C2278A19_AC86_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CRow;


#include "Profile.h"
#include "Cell.h"

class CCellProfile : public CProfile<CCell>  
{
public:
	CCellProfile(CRow* const pRow, long cellTotal);
	CCellProfile(CellVector vCells);
	virtual ~CCellProfile();

private:
	CRow* const m_pRow;		// Pointer is constant not the CRow

};

#endif // !defined(AFX_CELLPROFILE_H__C2278A19_AC86_11D3_BF15_004095086186__INCLUDED_)


