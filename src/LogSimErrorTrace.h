// LogSimErrorTrace.h: interface for the CLogSimErrorTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSIMERRORTRACE_H__97725C30_BF03_4B0E_83EF_D8EEE5D51754__INCLUDED_)
#define AFX_LOGSIMERRORTRACE_H__97725C30_BF03_4B0E_83EF_D8EEE5D51754__INCLUDED_


#include "LogErrorMessage.h"

class CLogSimErrorTrace : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSimErrorTrace(long time, const zString errStr);

	virtual ~CLogSimErrorTrace();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSimErrorTrace& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogSimErrorTrace(const CLogSimErrorTrace& oldMessage);
	CLogSimErrorTrace& operator=(const CLogSimErrorTrace& rhs);


	// ****************************************
	// Data members
private:

	const zString m_ErrorString;		// Error string to be output

};

#endif // !defined(AFX_LOGSIMERRORTRACE_H__97725C30_BF03_4B0E_83EF_D8EEE5D51754__INCLUDED_)
