// aaVectorProfile.h: interface for the aaVectorProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AAVECTORPROFILE_H__7F4A21B9_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AAVECTORPROFILE_H__7F4A21B9_B13C_11D3_BF19_004095086186__INCLUDED_


// Forward declarations

class CObservable;

// Include files

#include "aaVector.h"
#include "aaProfile.h"

class aaVectorProfile : public aaProfile <aaVector>
{
public:
	aaVectorProfile(long size);
	virtual ~aaVectorProfile();

	// The following indices, i, j refer to the element in the profile and the 
	// component of that element
	inline double GetData(long i, short int j)  const {return (GetDataSet().at(i))->GetData(j);}
	inline double GetData2(long i, short int j) const {return (GetDataSet().at(i))->GetData2(j);}
	inline void   ClearData() {DeleteData();}

	void AddData(CObservable* const pOb) const;

};

#endif // !defined(AFX_AAVECTORPROFILE_H__7F4A21B9_B13C_11D3_BF19_004095086186__INCLUDED_)
