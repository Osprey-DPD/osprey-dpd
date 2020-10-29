// LogpcProcessNotAccessible.h: interface for the CLogpcProcessNotAccessible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCPROCESSNOTACCESSIBLE_H__85DF75C1_287D_4EFC_995E_E29F7F78F26B__INCLUDED_)
#define AFX_LOGPCPROCESSNOTACCESSIBLE_H__85DF75C1_287D_4EFC_995E_E29F7F78F26B__INCLUDED_


#include "LogWarningMessage.h"

class CLogpcProcessNotAccessible : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcProcessNotAccessible(long time, const zString pid, const zString command);

	virtual ~CLogpcProcessNotAccessible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcProcessNotAccessible& rMsg);

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

	CLogpcProcessNotAccessible(const CLogpcProcessNotAccessible& oldMessage);
	CLogpcProcessNotAccessible& operator=(const CLogpcProcessNotAccessible& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;			// Target process' id
	const zString m_CommandName;	// Command name

};

#endif // !defined(AFX_LOGPCPROCESSNOTACCESSIBLE_H__85DF75C1_287D_4EFC_995E_E29F7F78F26B__INCLUDED_)
