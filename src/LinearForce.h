// LinearForce.h: interface for the CLinearForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEARFORCE_H__5CFF0944_0C66_11D4_BF30_004095086186__INCLUDED_)
#define AFX_LINEARFORCE_H__5CFF0944_0C66_11D4_BF30_004095086186__INCLUDED_


#include "ForceLaw.h"

class CLinearForce : public CForceLaw  
{
public:
	CLinearForce(long start, long end, long x, long y, long z, double rate);
	virtual ~CLinearForce();

	virtual double GetXForce(long time);	
	virtual double GetYForce(long time);	
	virtual double GetZForce(long time);	
													
private:
	double m_Rate;		// Rate at which force grows in time
};

#endif // !defined(AFX_LINEARFORCE_H__5CFF0944_0C66_11D4_BF30_004095086186__INCLUDED_)
