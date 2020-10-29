// LogShowAllProcesses.h: interface for the CLogShowAllProcesses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSHOWALLPROCESSES_H__60061527_489F_4842_B544_22A0124113D0__INCLUDED_)
#define AFX_LOGSHOWALLPROCESSES_H__60061527_489F_4842_B544_22A0124113D0__INCLUDED_


#include "LogInfoMessage.h" 

class CLogShowAllProcesses : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogShowAllProcesses(long time, ProcessSequence vProcesses);

	virtual ~CLogShowAllProcesses();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogShowAllProcesses& rMsg);

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

	CLogShowAllProcesses(const CLogShowAllProcesses& oldMessage);
	CLogShowAllProcesses& operator=(const CLogShowAllProcesses& rhs);

	// ****************************************
	// Data members
private:

	const long m_ProcessTotal;			// Number of running processes
	ProcessSequence m_Processes;		// Sequence of running processes
};

#endif // !defined(AFX_LOGSHOWALLPROCESSES_H__60061527_489F_4842_B544_22A0124113D0__INCLUDED_)
