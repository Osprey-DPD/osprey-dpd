// SineForce.h: interface for the CSineForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINEFORCE_H__5CFF0946_0C66_11D4_BF30_004095086186__INCLUDED_)
#define AFX_SINEFORCE_H__5CFF0946_0C66_11D4_BF30_004095086186__INCLUDED_


#include "ForceLaw.h"

class CSineForce : public CForceLaw  
{
public:
	CSineForce(long start, long end, long x, long y, long z, double amp, double period);
	virtual ~CSineForce();

	virtual double GetXForce(long time);	
	virtual double GetYForce(long time);	
	virtual double GetZForce(long time);	
													
private:
	double m_Amplitude;		// Amplitude of sinusoidal force
	double   m_Period;		// Period of force in time steps
	double m_Frequency;		// Frequency of sinusoidal force
};

#endif // !defined(AFX_SINEFORCE_H__5CFF0946_0C66_11D4_BF30_004095086186__INCLUDED_)
