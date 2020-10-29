// ScalarArrayObservable.h: interface for the CScalarArrayObservable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCALARARRAYOBSERVABLE_H__EC62FB33_3C4F_11D4_BF3C_004095086186__INCLUDED_)
#define AFX_SCALARARRAYOBSERVABLE_H__EC62FB33_3C4F_11D4_BF3C_004095086186__INCLUDED_


#include "Observable.h"

class CScalarArrayObservable : public CObservable  
{
public:
	CScalarArrayObservable(zString name, long analysisPeriod, long samplePeriod, long size);
	virtual ~CScalarArrayObservable();

	friend zOutStream& operator<<(zOutStream& os, const CScalarArrayObservable& rOb);

protected:
	virtual void Normalize(long currentTime);
	virtual void Serialize(zOutStream& os) const;
	virtual void DisplayDebug();
};

#endif // !defined(AFX_SCALARARRAYOBSERVABLE_H__EC62FB33_3C4F_11D4_BF3C_004095086186__INCLUDED_)
