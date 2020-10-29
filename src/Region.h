// Region.h: interface for the CRegion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGION_H__C2278A1A_AC86_11D3_BF15_004095086186__INCLUDED_)
#define AFX_REGION_H__C2278A1A_AC86_11D3_BF15_004095086186__INCLUDED_


// Include file to gain access to the typedefs

#include "xxBase.h"


// Forward declarations

class CAbstractBead;


class CRegion  
{
public:
	CRegion(long id);
	CRegion(long id, double width, double depth, double height);
	CRegion(const CRegion& oldRegion);
	virtual ~CRegion();

	inline long		GetId()			const {return m_id;}
	inline long		GetBeadTotal()	const {return m_lBeads.size();}
	inline double	GetWidth()		const {return m_Width;}
	inline double	GetDepth()		const {return m_Depth;}
	inline double	GetHeight()		const {return m_Height;}
	inline double	GetVolume()		const {return m_Volume;}
	inline BeadList GetBeads() const {return m_lBeads;}

	inline void     AddBead(CAbstractBead* pBead) {m_lBeads.push_back(pBead);}

protected:
	long m_id;							// Identifier for a region
	double m_Width;						
	double m_Depth;
	double m_Height;
	double m_Volume;
	BeadList m_lBeads;					// Beads in current region

};

#endif // !defined(AFX_REGION_H__C2278A1A_AC86_11D3_BF15_004095086186__INCLUDED_)
