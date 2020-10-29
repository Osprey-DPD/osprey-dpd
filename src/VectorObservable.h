// VectorObservable.h: interface for the CVectorObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTOROBSERVABLE_H__5652BEC3_5495_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_VECTOROBSERVABLE_H__5652BEC3_5495_11D3_820E_0060088AD300__INCLUDED_


#include "Observable.h"

class CVectorObservable : public CObservable
{
	friend class CMonitor;	// allow CMonitor to add data to the observable

public:
	CVectorObservable(zString name, long analysisPeriod, long samplePeriod);
	virtual ~CVectorObservable();

	void AddData(const aaVectorSingle* const pID);

	friend zOutStream& operator<<(zOutStream& os, const CVectorObservable& rOb);

protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	void DisplayDebug();

private:
	// Arrays to hold the vector data. 

	double m_SumData[3];
	double m_Sum2Data[3];
	double m_MeanData[3];
	double m_Mean2Data[3];
	double m_VarData[3];
	double m_SDevData[3];

	double m_SumMag;
	double m_Sum2Mag;
	double m_MeanMag;
	double m_Mean2Mag;
	double m_VarMag;
	double m_SDevMag;


};

#endif // !defined(AFX_VECTOROBSERVABLE_H__5652BEC3_5495_11D3_820E_0060088AD300__INCLUDED_)
