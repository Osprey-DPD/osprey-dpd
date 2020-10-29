// xxEventObject.h: interface for the xxEventObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXEVENTOBJECT_H__9757059E_B699_4DA3_AF85_95D8EE36CAB5__INCLUDED_)
#define AFX_XXEVENTOBJECT_H__9757059E_B699_4DA3_AF85_95D8EE36CAB5__INCLUDED_

// Forward declarations

class xxEvent;


// Include file to gain access to the STL container classes 

#include "xxBase.h"



class xxEventObject  
{
public:
	xxEventObject(const zString type);
	virtual ~xxEventObject();

	friend zOutStream& operator<<(zOutStream& os, const xxEventObject& EventOb);
	friend zInStream&  operator>>(zInStream& is,        xxEventObject& EventOb);

	long GetStartTime() const;
	long GetEndTime()   const;

	xxEvent* GetEvent() const;	// xxEvents cannot be const like xxCommands
	bool IsEventValid() const;

private:
	void SetEventValid(bool bValid);

private:
	xxEvent* m_pEvent;
	bool m_bEventValid;			// Flag showing if event data is valid

};

#endif // !defined(AFX_XXEVENTOBJECT_H__9757059E_B699_4DA3_AF85_95D8EE36CAB5__INCLUDED_)
