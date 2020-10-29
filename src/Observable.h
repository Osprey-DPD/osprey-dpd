// Observable.h: interface for the CObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBSERVABLE_H__5652BEC1_5495_11D3_820E_0060088AD300__INCLUDED_)
#define AFX_OBSERVABLE_H__5652BEC1_5495_11D3_820E_0060088AD300__INCLUDED_


// Include file needed by all derived classes.

#include "SimMathFlags.h"

// Forward declarations

class aaScalarSingle;
class aaVectorSingle;
class aaTensorSingle;
class aaScalarProfile;
class aaVectorProfile;

#include "xxBase.h"

class CObservable : public xxBase
{
	friend class CMonitor;
	friend class CAnalysis;
	friend class CAnalysisState;

public:
	CObservable(zString name, long analysisPeriod, long samplePeriod);
	virtual ~CObservable();


	friend zOutStream& operator<<(zOutStream& os, const CObservable& rOb);

	inline const zString GetName() const {return m_Name;}
	inline long GetTime() const {return m_CurrentTime;}

protected:
	// Pure virtual function to normalize the observable data that must be provided
	// by every derived class.

	virtual void Normalize(long currentTime) = 0; 

	// Pure virtual function to allow each derived class to serialize its data
	// to file. Note that this version is different from the version derived
	// from xxFile so CObservable cannot inherit from xxState or higher classes.
	// It needs to be different so that it can force a derived class to handle its
	// own serialization when invoked on a CObservable base class pointer.

	virtual	void Serialize(zOutStream& os) const = 0;

	// debug display routine to print out data values

	virtual void DisplayDebug();


protected:
	double m_SampleSize;
	long m_CurrentTime;

private:
	const zString m_Name;
	long m_AnalysisPeriod;
	long m_SamplePeriod;

};

#endif // !defined(AFX_OBSERVABLE_H__5652BEC1_5495_11D3_820E_0060088AD300__INCLUDED_)
