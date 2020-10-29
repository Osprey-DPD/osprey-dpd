// aaScalarSingle.h: interface for the aaScalarSingle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AASCALARSINGLE_H__7F4A21BD_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AASCALARSINGLE_H__7F4A21BD_B13C_11D3_BF19_004095086186__INCLUDED_


// Frdward declarations

class CObservable;

// Include files

#include "aaScalar.h"
#include "aaSingle.h"

class aaScalarSingle : public aaSingle<aaScalar> 
{
public:
	aaScalarSingle(double x);
	virtual ~aaScalarSingle();

	inline double GetData()  const {return GetDataSet().GetData();}
	inline double GetData2() const {return GetDataSet().GetData2();}

	void AddData(CObservable* const pOb) const;
};

#endif // !defined(AFX_AASCALARSINGLE_H__7F4A21BD_B13C_11D3_BF19_004095086186__INCLUDED_)
