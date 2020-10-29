// ScalarProfileObservable.h: interface for the CScalarProfileObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALARPROFILEOBSERVABLE_H__AF6B6517_B2EF_11D3_BF19_004095086186__INCLUDED_)
#define AFX_SCALARPROFILEOBSERVABLE_H__AF6B6517_B2EF_11D3_BF19_004095086186__INCLUDED_


// STL include files


#include "Observable.h"

class CScalarProfileObservable : public CObservable  
{
public:
	CScalarProfileObservable(zString name, long analysisPeriod, long samplePeriod, long size);
	virtual ~CScalarProfileObservable();

	void AddData(const aaScalarProfile* const pID);

	friend zOutStream& operator<<(zOutStream& os, const CScalarProfileObservable& rOb);

protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	void DisplayDebug();

private:
	// Vectors to hold the set of scalar data. 

	const long m_Size;		// Don't allow this to change else the AddData may fail

	zDoubleVector m_SumData;
	zDoubleVector m_Sum2Data;
	zDoubleVector m_MeanData;
	zDoubleVector m_SDevData;

};

#endif // !defined(AFX_SCALARPROFILEOBSERVABLE_H__AF6B6517_B2EF_11D3_BF19_004095086186__INCLUDED_)
