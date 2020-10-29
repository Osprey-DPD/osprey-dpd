// LogShowModifiableProcesses.h: interface for the CLogShowModifiableProcesses class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSHOWMODIFIABLEPROCESSES_H__34888AC0_22E7_4AA6_A472_1475F384D9BE__INCLUDED_)
#define AFX_LOGSHOWMODIFIABLEPROCESSES_H__34888AC0_22E7_4AA6_A472_1475F384D9BE__INCLUDED_


#include "LogInfoMessage.h" 

class CLogShowModifiableProcesses : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogShowModifiableProcesses(long time, ProcessSequence vProcesses);

	virtual ~CLogShowModifiableProcesses();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogShowModifiableProcesses& rMsg);

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

	CLogShowModifiableProcesses(const CLogShowModifiableProcesses& oldMessage);
	CLogShowModifiableProcesses& operator=(const CLogShowModifiableProcesses& rhs);

	// ****************************************
	// Data members
private:

	const long m_ProcessTotal;			// Number of modifiable processes
	ProcessSequence m_Processes;		// Sequence of modifiable processes
};

#endif // !defined(AFX_LOGSHOWMODIFIABLEPROCESSES_H__34888AC0_22E7_4AA6_A472_1475F384D9BE__INCLUDED_)
