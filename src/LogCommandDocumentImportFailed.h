// LogCommandDocumentImportFailed.h: interface for the CLogCommandDocumentImportFailed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOMMANDDOCUMENTIMPORTFAILED_H__6BEE049A_1EE1_46BD_AEBA_9E99A3960C15__INCLUDED_)
#define AFX_LOGCOMMANDDOCUMENTIMPORTFAILED_H__6BEE049A_1EE1_46BD_AEBA_9E99A3960C15__INCLUDED_


#include "LogErrorMessage.h"

class CLogCommandDocumentImportFailed : public CLogErrorMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCommandDocumentImportFailed(long time, const zString pid, const zString docName);

	virtual ~CLogCommandDocumentImportFailed();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCommandDocumentImportFailed& rMsg);

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

	CLogCommandDocumentImportFailed(const CLogCommandDocumentImportFailed& oldMessage);
	CLogCommandDocumentImportFailed& operator=(const CLogCommandDocumentImportFailed& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;	// Target process' id
	const zString m_Name;	// Command document's name including the ".xml" suffix
};


#endif // !defined(AFX_LOGCOMMANDDOCUMENTIMPORTFAILED_H__6BEE049A_1EE1_46BD_AEBA_9E99A3960C15__INCLUDED_)
