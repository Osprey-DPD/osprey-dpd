// FourierTransform.h: interface for the CFourierTransform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURIERTRANSFORM_H__A4A9FAA3_3585_11D4_BF3A_004095086186__INCLUDED_)
#define AFX_FOURIERTRANSFORM_H__A4A9FAA3_3585_11D4_BF3A_004095086186__INCLUDED_


#include "xxBase.h"

class CFourierTransform : public xxBase  
{
public:
	CFourierTransform();
	virtual ~CFourierTransform();

protected:
// Function to swap the two data items passed in by address.

	inline void Swap(double* px, double* py) {double temp = *px; *px = *py; *py = temp;}

};

#endif // !defined(AFX_FOURIERTRANSFORM_H__A4A9FAA3_3585_11D4_BF3A_004095086186__INCLUDED_)
