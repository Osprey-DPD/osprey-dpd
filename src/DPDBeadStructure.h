// DPDBeadStructure.h: interface for the CDPDBeadStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DPDBEADSTRUCTURE_H__2CE23034_D27E_11D3_BF1F_004095086186__INCLUDED_)
#define AFX_DPDBEADSTRUCTURE_H__2CE23034_D27E_11D3_BF1F_004095086186__INCLUDED_


#include "BeadStructure.h"

class CDPDBeadStructure : public CBeadStructure  
{
public:

    // Standard constructor that sets the radius of all beads = 0.5

	CDPDBeadStructure(zDoubleVector consInt, zDoubleVector dissInt);

    // Constructor that allows each bead to have a different radius

	CDPDBeadStructure(double radius, zDoubleVector consInt, zDoubleVector dissInt);

    // Constructor for Liquid-Gas DPD that requires two radii for the beads
    // and an extra set of conservative interaction parameters

	CDPDBeadStructure(double radius, double lgRadius, zDoubleVector consInt, 
                      zDoubleVector dissInt, zDoubleVector lgInt);

	virtual ~CDPDBeadStructure();

	double GetRadius() const;
	double GetLGRadius() const;
	const zDoubleVector& GetConsInt() const;
	const zDoubleVector& GetDissInt() const;
	const zDoubleVector& GetLGInt() const;

private:
	// The structure of standard DPD beads is their radius of interaction and their 
	// interactions with other beads: these take the form of conservative and 
	// dissipative forces. If the radius is not specified in the constructor
	// it is set to 0.5.

    // Liquid-Gas DPD requires an extra radius for the density-dependent force
    // and an extra set of conservative bead-bead parameters for the coefficents
    // of the density-dependent force.

	double m_Radius;
    double m_LGRadius;

	zDoubleVector m_vConsInt;
	zDoubleVector m_vDissInt;
    zDoubleVector m_vLGInt;

};

#endif // !defined(AFX_DPDBEADSTRUCTURE_H__2CE23034_D27E_11D3_BF1F_004095086186__INCLUDED_)
