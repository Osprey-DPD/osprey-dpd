// BDBeadStructure.h: interface for the CBDBeadStructure class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDBEADSTRUCTURE_H__A4B47E91_002D_4898_AB57_DC4288F53351__INCLUDED_)
#define AFX_BDBEADSTRUCTURE_H__A4B47E91_002D_4898_AB57_DC4288F53351__INCLUDED_


#include "BeadStructure.h"

class CBDBeadStructure : public CBeadStructure  
{
public:
	CBDBeadStructure(double radius, double transDiff, double rotDiff, zDoubleVector consInt, zDoubleVector dissInt);
	virtual ~CBDBeadStructure();

	double GetRadius() const;
	double GetTransDiff() const;
	double GetRotDiff() const;
	const zDoubleVector& GetConsInt() const;
	const zDoubleVector& GetDissInt() const;

private:
	// The structure of  BD beads contains their translational
    // and rotational diffusion coefficients and their interactions with other beads.
    // For now we use the same force laws as for DPD and specifiy their interaction
    // radius.

	double m_Radius;          // Bead interaction radius used in DPD force law
	double m_TransDiffusion;  // Isotropic translational diffusion coefficent
	double m_RotDiffusion;    // Isotropic rotational diffusion coefficient

	zDoubleVector m_vConsInt;
	zDoubleVector m_vDissInt;
};

#endif // !defined(AFX_BDBEADSTRUCTURE_H__A4B47E91_002D_4898_AB57_DC4288F53351__INCLUDED_)
