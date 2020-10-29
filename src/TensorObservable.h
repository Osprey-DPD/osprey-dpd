// TensorObservable.h: interface for the CTensorObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TENSOROBSERVABLE_H__5652BEC2_5495_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_TENSOROBSERVABLE_H__5652BEC2_5495_11D3_820E_0060088AD300__INCLUDED_



#include "Observable.h"

class CTensorObservable : public CObservable
{
	friend class CMonitor;	// allow CMonitor to add data to the observable

public:
	CTensorObservable(zString name, long analysisPeriod, long samplePeriod);
	virtual ~CTensorObservable();

	void AddData(const aaTensorSingle* const pID);

	friend zOutStream& operator<<(zOutStream& os, const CTensorObservable& rOb);


protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	void DisplayDebug();

private:
	// Arrays to hold the tensor data.

	double m_SumData[9];
	double m_Sum2Data[9];
	double m_MeanData[9];
	double m_Mean2Data[9];
	double m_VarData[9];
	double m_SDevData[9];
};

#endif // !defined(AFX_TENSOROBSERVABLE_H__5652BEC2_5495_11D3_820E_0060088AD300__INCLUDED_)
