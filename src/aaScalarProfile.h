// aaScalarProfile.h: interface for the aaScalarProfile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASCALARPROFILE_H__7F4A21B7_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AASCALARPROFILE_H__7F4A21B7_B13C_11D3_BF19_004095086186__INCLUDED_


// Forward declarations

class CScalarProfileObservable;

// Include files

#include "aaScalar.h"
#include "aaProfile.h"

class aaScalarProfile : public aaProfile<aaScalar>
{
public:
	aaScalarProfile(long size);
	virtual ~aaScalarProfile();

	// The following index refers to the element in the profile not the 
	// component of the element
	inline double GetData(long i)  const {return GetDataSet().at(i)->GetData();}
	inline double GetData2(long i) const {return GetDataSet().at(i)->GetData2();}
	inline void   ClearData() {DeleteData();}

	void AddData(CObservable* const pOb) const;

};

#endif // !defined(AFX_AASCALARPROFILE_H__7F4A21B7_B13C_11D3_BF19_004095086186__INCLUDED_)
