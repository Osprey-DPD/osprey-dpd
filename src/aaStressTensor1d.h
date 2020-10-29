// aaStressTensor1d.h: interface for the aaStressTensor1d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASTRESSTENSOR1D_H__EBEC6161_A80F_11D4_910C_0060088AD300__INCLUDED_)
#define AFX_AASTRESSTENSOR1D_H__EBEC6161_A80F_11D4_910C_0060088AD300__INCLUDED_


// Forward declarations

class CSliceProfile;


#include "aaStressTensor.h"

class aaStressTensor1d : public aaStressTensor  
{
public:
	aaStressTensor1d(const ISimBox* const pISimBox, CSliceProfile* const pProfile, long xn, long yn, long zn);

	virtual ~aaStressTensor1d();

	virtual void CalculateProfile();

private:
	CSliceProfile* const m_pProfile;		// Vector of CSlices across SimBox

	short  m_SliceNormalIndex;	// 0, 1 0r 2 depending on slice normal
	double m_SliceFactor;		// Constants relating to the slice size

	double m_Config[9];			// Configurational contribution to the stress tensor
	double m_Kinetic[9];		// Kinetic contribution
};

#endif // !defined(AFX_AASTRESSTENSOR1D_H__EBEC6161_A80F_11D4_910C_0060088AD300__INCLUDED_)
