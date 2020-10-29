// SpringForce.h: interface for the CSpringForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRINGFORCE_H__24914BF5_30AF_11D4_BF39_004095086186__INCLUDED_)
#define AFX_SPRINGFORCE_H__24914BF5_30AF_11D4_BF39_004095086186__INCLUDED_


#include "ForceLaw.h"

class CSpringForce : public CForceLaw  
{
public:
	CSpringForce(long start, long end, long x, long y, long z, 
				 double springConst, double springLength, double velocity,
				 BeadVector beads);
	virtual ~CSpringForce();

	virtual double GetXForce(long time);	
	virtual double GetYForce(long time);	
	virtual double GetZForce(long time);	

private:
	double m_SpringConst;
	double m_SpringLength;
	double m_Velocity;

	double m_Length;		// Instantaneous spring length

	double m_TransXPos;
	double m_TransYPos;		// Current coordinates of the transducer
	double m_TransZPos;

	double m_TransXPos0;
	double m_TransYPos0;	// Initial coordinates of the transducer
	double m_TransZPos0;

	double m_TargetXPos;
	double m_TargetYPos;	// Current coordinates of the target
	double m_TargetZPos;

	double m_TargetSize;	// Number of beads in target

	BeadVector m_Beads;		// Vector of beads in target

};

#endif // !defined(AFX_SPRINGFORCE_H__24914BF5_30AF_11D4_BF39_004095086186__INCLUDED_)
