// aaVectorSingle.h: interface for the aaVectorSingle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AAVECTORSINGLE_H__7F4A21BE_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AAVECTORSINGLE_H__7F4A21BE_B13C_11D3_BF19_004095086186__INCLUDED_


// Forward declarations

class CObservable;

// Include files

#include "aaVector.h"
#include "aaSingle.h"

class aaVectorSingle  : public aaSingle<aaVector>
{
public:
	aaVectorSingle(double x, double y, double z);
	virtual ~aaVectorSingle();

	inline double GetData(short int i)  const {return GetDataSet().GetData(i);}
	inline double GetData2(short int i) const {return GetDataSet().GetData2(i);}

	void AddData(CObservable* const pOb) const;

};

#endif // !defined(AFX_AAVECTORSINGLE_H__7F4A21BE_B13C_11D3_BF19_004095086186__INCLUDED_)
