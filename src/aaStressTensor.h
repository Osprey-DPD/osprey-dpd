// aaStressTensor.h: interface for the aaStressTensor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASTRESSTENSOR_H__EBEC6160_A80F_11D4_910C_0060088AD300__INCLUDED_)
#define AFX_AASTRESSTENSOR_H__EBEC6160_A80F_11D4_910C_0060088AD300__INCLUDED_


// Forward declarations

class ISimBox;
class aaStressTensorPoint;


// Include files

#include "xxBase.h"

class aaStressTensor  : public xxBase
{
public:
	aaStressTensor(const ISimBox* const pISimBox, long xn, long yn, long zn);

	virtual ~aaStressTensor();

	// Access functions

	// Normal vector to the stress grid.

	inline long	  GetXNormal()		const {return m_Normal[0];}
	inline long   GetYNormal()		const {return m_Normal[1];}
	inline long   GetZNormal()		const {return m_Normal[2];}
	inline long   GetNormalIndex()	const {return m_NormalIndex;}

	// The size of the stress grid profile, and the width 
	// and volume of each element: these must be set by the derived classes.

	inline double GetSize()		const {return m_StressGridSize;}
	inline double GetWidth()	const {return m_Width;}
	inline double GetVolume()	const {return m_Volume;}

	// Functions to return the components of the stress tensor stored in the
	// internal profile of aaStressTensorPoints. The slice index and bead pair 
	// index must be supplied. Note that only the configuration contribution 
	// is returned by the aaStressTensorPoint class for now.

	double GetComponent(long index, long component) const;


	double GetBeadStress11(long slice, long beadPair) const;
	double GetBeadStress12(long slice, long beadPair) const;
	double GetBeadStress13(long slice, long beadPair) const;
	double GetBeadStress21(long slice, long beadPair) const;
	double GetBeadStress22(long slice, long beadPair) const;
	double GetBeadStress23(long slice, long beadPair) const;
	double GetBeadStress31(long slice, long beadPair) const;
	double GetBeadStress32(long slice, long beadPair) const;
	double GetBeadStress33(long slice, long beadPair) const;

	// Functions to return iterators to the profile of aaStressTensorPoint objects
	// allowing the STL transform() function to convert the data into aaScalars, etc.

	inline cStressVectorIterator Begin() const {return m_vProfile.begin();}
	inline cStressVectorIterator End()   const {return m_vProfile.end();}

	// Pure virtual function that must be provided by derived classes
	// to calculate the stress tensor averaged over their regions

	virtual void CalculateProfile() = 0;


protected:

	// Functions changing the stress grid profile can only be accessed by
	// derived classes.

	void AddStressTensorPoint();
	void AddBeadStress(long gridIndex, long beadIndex, const double con[9], const double kin[9]) const;
	void AddBondStress(long gridIndex, long bondIndex, const double con[9], const double kin[9]) const;
	void AddBondPairStress(long gridIndex, long bondPairIndex, const double con[9], const double kin[9]) const;
	void MultiplyStressTensor(double conFactor, double kinFactor) const;

protected:

	ISimBox const* const m_pISimBox;	// Pointer to the ISimBox to get access to CMonitor data

	long m_BeadTypeTotal;		// Number of bead types
	long m_BondTypeTotal;		// Number of bond types
	long m_BondPairTypeTotal;	// Number of bondpair types
	long m_BeadPairTypeTotal;	// Number of bead-bead pair types
	long m_Normal[3];			// Normal to stress profile
	long m_NormalIndex;			// Integer indicating the profile normal
	long m_StressGridSize;		// Number of slices over which stress tensor is averaged

	double m_Width;				// Spacing that depends on the normal direction
	double m_Volume;			// Volume of region over which stress is averaged

	StressVector m_vProfile;	// Vector of aaStressTensorPoint pointers
};

#endif // !defined(AFX_AASTRESSTENSOR_H__EBEC6160_A80F_11D4_910C_0060088AD300__INCLUDED_)
