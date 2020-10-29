// FT1d.h: interface for the CFT1d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FT1D_H__A4A9FAA4_3585_11D4_BF3A_004095086186__INCLUDED_)
#define AFX_FT1D_H__A4A9FAA4_3585_11D4_BF3A_004095086186__INCLUDED_


#include "FourierTransform.h"

// Rename the valarray to a local name for use in the Fourier transform routines

typedef zDoubleValArray		array1d;


class CFT1d : public CFourierTransform  
{
public:
	CFT1d(long size);
	virtual ~CFT1d();


private:

};

#endif // !defined(AFX_FT1D_H__A4A9FAA4_3585_11D4_BF3A_004095086186__INCLUDED_)
