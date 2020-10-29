// WallData.h: interface for the CWallData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WALLDATA_H__62397B53_8852_11D3_BF0C_004095086186__INCLUDED_)
#define AFX_WALLDATA_H__62397B53_8852_11D3_BF0C_004095086186__INCLUDED_


#include "xxConstraint.h"

class CWallData : public xxConstraint
{
public:
	CWallData();
	~CWallData();

	CWallData(const CWallData& oldWD);				// copy constructor
	CWallData& operator=(const CWallData& rWD);		// assignment operator

	friend zOutStream& operator<<(zOutStream& os, const CWallData& wall);
	friend zInStream&  operator>>(zInStream&  is,       CWallData& wall);

	friend bool operator ==(const CWallData &a, const CWallData &b);

	const zString GetWallPolymerName() const;
	double GetWallBeadDensity() const;
	long GetWallXWidth() const;
	long GetWallYWidth() const;
	long GetWallZWidth() const;

private:
	zString m_PolymerName;		// polymer with its head stuck in the wall
	double m_Density;			// bead density of head bead in wall volume
	long m_XWidth;				// number of CNT cells in wall's width on both faces
	long m_YWidth;			
	long m_ZWidth;			
};

#endif // !defined(AFX_WALLDATA_H__62397B53_8852_11D3_BF0C_004095086186__INCLUDED_)
