// xxProcessObject.h: interface for the xxProcessObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXPROCESSOBJECT_H__3E58920E_4F8F_4857_8B8E_38359B0735FA__INCLUDED_)
#define AFX_XXPROCESSOBJECT_H__3E58920E_4F8F_4857_8B8E_38359B0735FA__INCLUDED_

// Foward declarations

class xxProcess;

// Include file to gain access to the STL container classes 

#include "xxBase.h"


class xxProcessObject  
{
public:
	xxProcessObject(const zString type);
	virtual ~xxProcessObject();

	friend zOutStream& operator<<(zOutStream& os, const xxProcessObject& ProcessOb);
	friend zInStream&  operator>>(zInStream& is,        xxProcessObject& ProcessOb);

	long GetStartTime() const;
	long GetEndTime()   const;

	xxProcess* GetProcess() const;	// xxProcesses cannot be const like xxCommands
	bool IsProcessValid() const;

private:
	void SetProcessValid(bool bValid);

private:
	xxProcess* m_pProcess;
	bool m_bProcessValid;			// Flag showing if process data is valid


};

#endif // !defined(AFX_XXPROCESSOBJECT_H__3E58920E_4F8F_4857_8B8E_38359B0735FA__INCLUDED_)
