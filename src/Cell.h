// Cell.h: interface for the CCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELL_H__C2278A1D_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_CELL_H__C2278A1D_AC86_11D3_BF15_004095086186__INCLUDED_


#include "Region.h"

class CCell : public CRegion  
{
public:
	CCell(long id, double width, double depth, double height);
	CCell(const CCell& oldCell);
	virtual ~CCell();

};

#endif // !defined(AFX_CELL_H__C2278A1D_AC86_11D3_BF15_004095086186__INCLUDED_)
