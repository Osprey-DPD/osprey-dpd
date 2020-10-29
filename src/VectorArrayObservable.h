// VectorArrayObservable.h: interface for the CVectorArrayObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VECTORARRAYOBSERVABLE_H__EC62FB34_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_VECTORARRAYOBSERVABLE_H__EC62FB34_3C4F_11D4_BF3C_004095086186__INCLUDED_


#include "Observable.h"

class CVectorArrayObservable : public CObservable  
{
public:
	CVectorArrayObservable(zString name, long analysisPeriod, long samplePeriod, long size);
	virtual ~CVectorArrayObservable();

	friend zOutStream& operator<<(zOutStream& os, const CVectorArrayObservable& rOb);

protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	virtual void DisplayDebug();
};

#endif // !defined(AFX_VECTORARRAYOBSERVABLE_H__EC62FB34_3C4F_11D4_BF3C_004095086186__INCLUDED_)
