// Row.h: interface for the CRow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROW_H__C2278A1C_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_ROW_H__C2278A1C_AC86_11D3_BF15_004095086186__INCLUDED_


// Forward declarations

class CSlice;


#include "Region.h"

class CRow : public CRegion  
{
public:
	CRow(long id, long xNormal, long yNormal, long zNormal, 
		 double width, double depth, double height);

	CRow(const CSlice& rSlice, long rowIndex);

	CRow(const CRow& oldRow);
	virtual ~CRow();

	inline long GetXNormal()  const {return m_X;}
	inline long GetYNormal()  const {return m_Y;}
	inline long GetZNormal()  const {return m_Z;}

private:
	long m_X;		// Normal to row: not the same as the origin slice's normal
	long m_Y;
	long m_Z;
};

#endif // !defined(AFX_ROW_H__C2278A1C_AC86_11D3_BF15_004095086186__INCLUDED_)
