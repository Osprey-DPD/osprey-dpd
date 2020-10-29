// aaTensorProfile.h: interface for the aaTensorProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AATENSORPROFILE_H__7F4A21BA_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AATENSORPROFILE_H__7F4A21BA_B13C_11D3_BF19_004095086186__INCLUDED_


// Include files

#include "aaTensor.h"
#include "aaProfile.h"

class aaTensorProfile : public aaProfile<aaTensor> 
{
public:
	aaTensorProfile(long size);
	virtual ~aaTensorProfile();

	// The following indices, i, j refer to the element in the profile and the 
	// component of that element
	inline double GetData(long i, short int j)  const {return GetDataSet().at(i)->GetData(j);}
	inline double GetData2(long i, short int j) const {return GetDataSet().at(i)->GetData2(j);}
	inline void   ClearData() {DeleteData();}

};

#endif // !defined(AFX_AATENSORPROFILE_H__7F4A21BA_B13C_11D3_BF19_004095086186__INCLUDED_)
