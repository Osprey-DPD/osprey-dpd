// ConstantForce.h: interface for the CConstantForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSTANTFORCE_H__5CFF0945_0C66_11D4_BF30_004095086186__INCLUDED_)
#define AFX_CONSTANTFORCE_H__5CFF0945_0C66_11D4_BF30_004095086186__INCLUDED_


#include "ForceLaw.h"

class CConstantForce : public CForceLaw  
{
public:
	CConstantForce(long start, long end, long x, long y, long z, double mag);
	virtual ~CConstantForce();

	virtual double GetXForce(long simTime);
	virtual double GetYForce(long simTime);
	virtual double GetZForce(long simTime);

private:
	double m_Magnitude;
};

#endif // !defined(AFX_CONSTANTFORCE_H__5CFF0945_0C66_11D4_BF30_004095086186__INCLUDED_)
