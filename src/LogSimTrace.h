// LogSimTrace.h: interface for the CLogSimTrace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSIMTRACE_H__0854B204_4BC9_48A1_BB64_BB389559AD93__INCLUDED_)
#define AFX_LOGSIMTRACE_H__0854B204_4BC9_48A1_BB64_BB389559AD93__INCLUDED_


#include "LogInfoMessage.h"

class CLogSimTrace : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSimTrace(long time, const zString infoStr);

	virtual ~CLogSimTrace();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSimTrace& rMsg);

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

	CLogSimTrace(const CLogSimTrace& oldMessage);
	CLogSimTrace& operator=(const CLogSimTrace& rhs);


	// ****************************************
	// Data members
private:

	const zString m_InfoString;		// Information string to be output
};

#endif // !defined(AFX_LOGSIMTRACE_H__0854B204_4BC9_48A1_BB64_BB389559AD93__INCLUDED_)
