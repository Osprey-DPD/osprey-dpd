// InitialStateObject.h: interface for the CInitialStateObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INITIALSTATEOBJECT_H__6CE669F3_04B5_11D4_BF30_004095086186__INCLUDED_)
#define AFX_INITIALSTATEOBJECT_H__6CE669F3_04B5_11D4_BF30_004095086186__INCLUDED_


// Forward declarations

class CInitialStateData;


// Include file to gain access to the STL container classes 

#include "xxBase.h"

class CInitialStateObject  
{
public:
	CInitialStateObject(const zString type);
	virtual ~CInitialStateObject();

	friend zOutStream& operator<<(zOutStream& os, const CInitialStateObject& ISDOb);
	friend zInStream&  operator>>(zInStream& is,        CInitialStateObject& ISDOb);

	bool IsDataValid()  const;
	const CInitialStateData* GetISD() const;

private:
	void SetDataValid(bool bValid);


private:
	CInitialStateData* m_pISD;	// Pointer to the initial state data object
	bool m_bISDValid;			// Flag showing if initial state data is valid

};

#endif // !defined(AFX_INITIALSTATEOBJECT_H__6CE669F3_04B5_11D4_BF30_004095086186__INCLUDED_)

