// ForceLaw.h: interface for the CForceLaw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FORCELAW_H__5CFF0943_0C66_11D4_BF30_004095086186__INCLUDED_)
#define AFX_FORCELAW_H__5CFF0943_0C66_11D4_BF30_004095086186__INCLUDED_


#include "xxBase.h"


class CForceLaw  
{
public:
	CForceLaw(long start, long end, long x, long y, long z);
	virtual ~CForceLaw();

	inline long GetStart() const {return m_Start;}
	inline long GetEnd()   const {return m_End;}
	
	virtual double GetXForce(long time)  = 0;	// PVFs that must be implemented by all
	virtual double GetYForce(long time)  = 0;	// derived classes to return the 
	virtual double GetZForce(long time)  = 0;	// components of the force
				
	// Function that must be overridden by derived classes whose force laws depend
	// on the bead coordinates

	virtual void CalculateVariableForces(const CAbstractBead* const pBead, double* fx, double* fy, double* fz);

protected:
	long m_Start;	// Time at which force is turned on
	long m_End;		// and off

	double m_X;
	double m_Y;		// Unit vector in the direction of force
	double m_Z;
};

#endif // !defined(AFX_FORCELAW_H__5CFF0943_0C66_11D4_BF30_004095086186__INCLUDED_)
