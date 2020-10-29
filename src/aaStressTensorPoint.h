// aaStressTensorPoint.h: interface for the aaStressTensorPoint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASTRESSTENSORPOINT_H__EBEC6162_A80F_11D4_910C_0060088AD300__INCLUDED_)
#define AFX_AASTRESSTENSORPOINT_H__EBEC6162_A80F_11D4_910C_0060088AD300__INCLUDED_


// Include files needed for vector container

#include "xxBase.h"


class aaStressTensorPoint  
{
public:
	aaStressTensorPoint(long beadTypes, long bondTypes, long bondPairTypes);
	virtual ~aaStressTensorPoint();

	// Functions to add a stress contribution to the current tensor from 
	// bead-bead interactions, bond forces and bondpair forces.

	void AddBeadStress(long index, const double config[9], const double kin[9]);
	void AddBondStress(long index, const double config[9], const double kin[9]);
	void AddBondPairStress(long index, const double config[9], const double kin[9]);

	void MultiplyStressTensor(double cx,double kx);

	// Function to return a bead-bead interaction index given the bead types

	inline long Index(long i, long j) const {return i*m_BeadTypeTotal + j;}

	// Functions to return the stress tensor contributions for a specified 
	// bead-bead pair or bond or bondpair.

	inline double GetBeadStress11(long index) const	{return m_vBeadCon11.at(index);}
	inline double GetBeadStress12(long index) const	{return m_vBeadCon12.at(index);}
	inline double GetBeadStress13(long index) const	{return m_vBeadCon13.at(index);}
	inline double GetBeadStress21(long index) const	{return m_vBeadCon21.at(index);}
	inline double GetBeadStress22(long index) const	{return m_vBeadCon22.at(index);}
	inline double GetBeadStress23(long index) const	{return m_vBeadCon23.at(index);}
	inline double GetBeadStress31(long index) const	{return m_vBeadCon31.at(index);}
	inline double GetBeadStress32(long index) const	{return m_vBeadCon32.at(index);}
	inline double GetBeadStress33(long index) const	{return m_vBeadCon33.at(index);}

	inline double GetBondStress11(long index) const	{return m_vBondCon11.at(index);}
	inline double GetBondStress12(long index) const	{return m_vBondCon12.at(index);}
	inline double GetBondStress13(long index) const	{return m_vBondCon13.at(index);}
	inline double GetBondStress21(long index) const	{return m_vBondCon21.at(index);}
	inline double GetBondStress22(long index) const	{return m_vBondCon22.at(index);}
	inline double GetBondStress23(long index) const	{return m_vBondCon23.at(index);}
	inline double GetBondStress31(long index) const	{return m_vBondCon31.at(index);}
	inline double GetBondStress32(long index) const	{return m_vBondCon32.at(index);}
	inline double GetBondStress33(long index) const	{return m_vBondCon33.at(index);}

	inline double GetBondPairStress11(long index) const	{return m_vBondPairCon11.at(index);}
	inline double GetBondPairStress12(long index) const	{return m_vBondPairCon12.at(index);}
	inline double GetBondPairStress13(long index) const	{return m_vBondPairCon13.at(index);}
	inline double GetBondPairStress21(long index) const	{return m_vBondPairCon21.at(index);}
	inline double GetBondPairStress22(long index) const	{return m_vBondPairCon22.at(index);}
	inline double GetBondPairStress23(long index) const	{return m_vBondPairCon23.at(index);}
	inline double GetBondPairStress31(long index) const	{return m_vBondPairCon31.at(index);}
	inline double GetBondPairStress32(long index) const	{return m_vBondPairCon32.at(index);}
	inline double GetBondPairStress33(long index) const	{return m_vBondPairCon33.at(index);}

	// Functions to return the contribution of a specified bead-bead interaction,
	// to the stress profile averaged over slices through the SimBox. 
	// The slice normal is indicated by the function name.

	double GetBeadStressXProfile(long index) const;
	double GetBeadStressYProfile(long index) const;
	double GetBeadStressZProfile(long index) const;

	double GetBondStressXProfile(long index) const;
	double GetBondStressYProfile(long index) const;
	double GetBondStressZProfile(long index) const;

	double GetBondPairStressXProfile(long index) const;
	double GetBondPairStressYProfile(long index) const;
	double GetBondPairStressZProfile(long index) const;

	// Functions to return components of the stress due to all bead-bead, 
	// bond and bondpair interactions. Notice the index into the 1d array 
	// m_Config[] maps onto the the 3 x 3 stress tensor.
	// We use the Schofield and Henderson definition that the surface tension
	// is the normal minus the (average) lateral component of the "pressure" tensor
	// according to Eq. 3.8 of Proc. R Soc. London A 379:231 (1982), and note that 
	// what we have called the stress tensor in the code is actually the pressure tensor.

	inline double GetComponent(long index)		const {return m_Config[index];}
	inline double GetXProfile()					const {return m_Config[0] - 0.5*(m_Config[4] + m_Config[8]);}
	inline double GetYProfile()					const {return m_Config[4] - 0.5*(m_Config[8] + m_Config[0]);}
	inline double GetZProfile()					const {return m_Config[8] - 0.5*(m_Config[0] + m_Config[4]);}

private:

	const long m_BeadTypeTotal;		// Number of bead types
	const long m_BondTypeTotal;		// Number of bond types
	const long m_BondPairTypeTotal;	// Number of bondpair types
	const long m_BeadPairTotal;		// Number of bead-bead pair types NOT bead types!

	zDoubleVector m_vBeadCon11;		// Configurational stress tensor components stored 
	zDoubleVector m_vBeadCon12;		// by bead-bead index
	zDoubleVector m_vBeadCon13;		//
	zDoubleVector m_vBeadCon21;		//   index = Type1*No of bead types + Type2
	zDoubleVector m_vBeadCon22;
	zDoubleVector m_vBeadCon23;
	zDoubleVector m_vBeadCon31;
	zDoubleVector m_vBeadCon32;
	zDoubleVector m_vBeadCon33;

	zDoubleVector m_vBondCon11;		// Bond contribution to the configurational
	zDoubleVector m_vBondCon12;		// stress tensor
	zDoubleVector m_vBondCon13;		//
	zDoubleVector m_vBondCon21;		//  index = bond type
	zDoubleVector m_vBondCon22;
	zDoubleVector m_vBondCon23;
	zDoubleVector m_vBondCon31;
	zDoubleVector m_vBondCon32;
	zDoubleVector m_vBondCon33;

	zDoubleVector m_vBondPairCon11;		// Bondpair contribution to the configurational
	zDoubleVector m_vBondPairCon12;		// stress tensor
	zDoubleVector m_vBondPairCon13;		//
	zDoubleVector m_vBondPairCon21;		//  index = bond type
	zDoubleVector m_vBondPairCon22;
	zDoubleVector m_vBondPairCon23;
	zDoubleVector m_vBondPairCon31;
	zDoubleVector m_vBondPairCon32;
	zDoubleVector m_vBondPairCon33;

	double m_Config[9];				// Total configurational stress tensor (bead-bead + bond + bondpair)
};

#endif // !defined(AFX_AASTRESSTENSORPOINT_H__EBEC6162_A80F_11D4_910C_0060088AD300__INCLUDED_)
