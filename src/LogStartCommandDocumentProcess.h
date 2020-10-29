// LogStartCommandDocumentProcess.h: interface for the CLogStartCommandDocumentProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSTARTCOMMANDDOCUMENTPROCESS_H__D06FAC23_C967_49C0_B073_B24500FF321E__INCLUDED_)
#define AFX_LOGSTARTCOMMANDDOCUMENTPROCESS_H__D06FAC23_C967_49C0_B073_B24500FF321E__INCLUDED_


#include "LogInfoMessage.h"

class CLogStartCommandDocumentProcess : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogStartCommandDocumentProcess(long time);

	virtual ~CLogStartCommandDocumentProcess();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogStartCommandDocumentProcess& rMsg);

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

	CLogStartCommandDocumentProcess(const CLogStartCommandDocumentProcess& oldMessage);
	CLogStartCommandDocumentProcess& operator=(const CLogStartCommandDocumentProcess& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGSTARTCOMMANDDOCUMENTPROCESS_H__D06FAC23_C967_49C0_B073_B24500FF321E__INCLUDED_)
