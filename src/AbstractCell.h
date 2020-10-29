// AbstractCell.h: interface for the CAbstractCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CELL_H__45E558C1_DA21_11D2_B61A_0060088AD300__INCLUDED_)
#define AFX_CELL_H__45E558C1_DA21_11D2_B61A_0060088AD300__INCLUDED_



#include "xxBase.h"


class CAbstractCell : public xxBase
{
public:
	CAbstractCell();
    CAbstractCell(const CAbstractCell& oldCell);
	virtual ~CAbstractCell();

public:
	inline long		GetId()         const {return m_id;}
	inline long		GetBLXIndex()   const {return m_BLIndex[0];}
	inline long		GetBLYIndex()   const {return m_BLIndex[1];} 
	inline long		GetBLZIndex()   const {return m_BLIndex[2];}
	inline double   GetBLXCoord()   const {return m_BLCoord[0];}
	inline double   GetBLYCoord()   const {return m_BLCoord[1];}
	inline double   GetBLZCoord()   const {return m_BLCoord[2];}
	inline double   GetTRXCoord()   const {return m_TRCoord[0];}
	inline double   GetTRYCoord()   const {return m_TRCoord[1];}
	inline double   GetTRZCoord()   const {return m_TRCoord[2];}
	
	inline BeadList GetBeads()		const {return m_lBeads;}

	inline void SetId(long id) {m_id = id;}


protected:

	long m_id;
	long m_BLIndex[3];
	double m_BLCoord[3];
	double m_TRCoord[3];
	BeadList m_lBeads;

};

#endif // !defined(AFX_CELL_H__45E558C1_DA21_11D2_B61A_0060088AD300__INCLUDED_)
