// VectorProfileObservable.h: interface for the CVectorProfileObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTORPROFILEOBSERVABLE_H__B69409C3_C36F_11D3_BF1B_004095086186__INCLUDED_)
#define AFX_VECTORPROFILEOBSERVABLE_H__B69409C3_C36F_11D3_BF1B_004095086186__INCLUDED_


// STL include files



#include "Observable.h"

class CVectorProfileObservable : public CObservable  
{
public:
	CVectorProfileObservable(zString name, long analysisPeriod, long samplePeriod, long size);
	virtual ~CVectorProfileObservable();

	virtual void AddData(const aaVectorProfile* const pID);

	friend zOutStream& operator<<(zOutStream& os, const CVectorProfileObservable& rOb);

protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	void DisplayDebug();

private:
	// Vectors to hold the set of vector data obtained from a slice profile across the SimBox

	const long m_Size;	// Don't allow this to change else AddData() may fail

	zDoubleVector m_SumX;
	zDoubleVector m_SumY;
	zDoubleVector m_SumZ;
	zDoubleVector m_SumMag;

	zDoubleVector m_SumX2;
	zDoubleVector m_SumY2;
	zDoubleVector m_SumZ2;
	zDoubleVector m_Sum2Mag;

	zDoubleVector m_MeanX;
	zDoubleVector m_MeanY;
	zDoubleVector m_MeanZ;
	zDoubleVector m_MeanMag;
	zDoubleVector m_SDevX;
	zDoubleVector m_SDevY;
	zDoubleVector m_SDevZ;
	zDoubleVector m_SDevMag;

};

#endif // !defined(AFX_VECTORPROFILEOBSERVABLE_H__B69409C3_C36F_11D3_BF1B_004095086186__INCLUDED_)
