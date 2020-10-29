// aaStressTensor2d.h: interface for the aaStressTensor2d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASTRESSTENSOR2D_H__2AA60241_A8FA_11D4_910C_0060088AD300__INCLUDED_)
#define AFX_AASTRESSTENSOR2D_H__2AA60241_A8FA_11D4_910C_0060088AD300__INCLUDED_


// Forward declarations

class CSlice;

#include "aaStressTensor.h"

class aaStressTensor2d : public aaStressTensor  
{
public:
	aaStressTensor2d(const ISimBox* const pISimBox, CSlice* const pProfile, 
					 long xn, long yn, long zn);
	virtual ~aaStressTensor2d();


	virtual void CalculateProfile();

private:

	CSlice* m_pSlice;	// A slice through the SimBox used to create a 2d se of cells
};

#endif // !defined(AFX_AASTRESSTENSOR2D_H__2AA60241_A8FA_11D4_910C_0060088AD300__INCLUDED_)
