// ScalarObservable.h: interface for the CScalarObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALAROBSERVABLE_H__5652BEC4_5495_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_SCALAROBSERVABLE_H__5652BEC4_5495_11D3_820E_0060088AD300__INCLUDED_


#include "Observable.h"


class CScalarObservable : public CObservable
{
	friend class CMonitor;	// allow CMonitor to add data to the observable

public:
	CScalarObservable(zString name, long analysisPeriod, long samplePeriod);
	virtual ~CScalarObservable();

	void AddData(const aaScalarSingle* const pID);

	friend zOutStream& operator<<(zOutStream& os, const CScalarObservable& rOb);

protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	void DisplayDebug();

private:
	// Arrays to hold the scalar data. 

	double m_SumData;
	double m_Sum2Data;
	double m_MeanData;
	double m_Mean2Data;
	double m_VarData;
	double m_SDevData;

};

#endif // !defined(AFX_SCALAROBSERVABLE_H__5652BEC4_5495_11D3_820E_0060088AD300__INCLUDED_)
