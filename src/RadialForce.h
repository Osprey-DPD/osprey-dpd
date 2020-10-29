// RadialForce.h: interface for the CRadialForce class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RADIALFORCE_H__F5FF7977_40E9_4AFB_AF11_FAEC17AFC3C7__INCLUDED_)
#define AFX_RADIALFORCE_H__F5FF7977_40E9_4AFB_AF11_FAEC17AFC3C7__INCLUDED_


#include "ForceLaw.h"

class CRadialForce : public CForceLaw  
{
public:
	CRadialForce(long start, long end, long x, long y, long z, 
				 double cx, double cy, double cz, double magnitude, BeadVector beads);
	virtual ~CRadialForce();

	// PVFs to allow the force to be calculated when it is independent
	// of the bead coordinates

	virtual double GetXForce(long time);
	virtual double GetYForce(long time);
	virtual double GetZForce(long time);

	// Function to calculate the force when it depends on the bead coordinates

	virtual void CalculateVariableForces(const CAbstractBead* const pBead, double* fx, double* fy, double* fz);

private:

	double m_CentreX;
	double m_CentreY;		// Force origin in units of bead diameters
	double m_CentreZ;
	double m_Magnitude;		// Magnitude of the (constant) force
	

	// Local data needed to calculate the radial force

	double m_RelOX;
	double m_RelOY;			// Bead position vector - force origin vector
	double m_RelOZ;
	double m_RelDotN;		// Scalar product of relative bead vector and force plane normal
	double m_ForceX;
	double m_ForceY;		// Rotating vector in the plane of the force
	double m_ForceZ;
	double m_ForceMag;		// Magnitude of vector in direction of force

	double m_TargetSize;	// Number of beads in target

	BeadVector m_Beads;		// Vector of beads in target

};
#endif // !defined(AFX_RADIALFORCE_H__F5FF7977_40E9_4AFB_AF11_FAEC17AFC3C7__INCLUDED_)
