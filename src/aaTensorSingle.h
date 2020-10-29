// aaTensorSingle.h: interface for the aaTensorSingle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AATENSORSINGLE_H__7F4A21BF_B13C_11D3_BF19_004095086186__INCLUDED_)
#define AFX_AATENSORSINGLE_H__7F4A21BF_B13C_11D3_BF19_004095086186__INCLUDED_


// Forward declarations

class CObservable;

// Include files

#include "aaTensor.h"
#include "aaSingle.h"

class aaTensorSingle : public aaSingle<aaTensor>  
{
public:
	aaTensorSingle(const double* const pData);
	virtual ~aaTensorSingle();

	double GetData(short int i)  const {return GetDataSet().GetData(i);}
	double GetData2(short int i) const {return GetDataSet().GetData2(i);}

	void AddData(CObservable* const pOb) const;
};

#endif // !defined(AFX_AATENSORSINGLE_H__7F4A21BF_B13C_11D3_BF19_004095086186__INCLUDED_)
