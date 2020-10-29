// LogpcCommandDocumentAddDocument.h: interface for the CLogpcCommandDocumentAddDocument class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCCOMMANDDOCUMENTADDDOCUMENT_H__3DD82B3E_78D6_4999_8BEB_6F723F684B40__INCLUDED_)
#define AFX_LOGPCCOMMANDDOCUMENTADDDOCUMENT_H__3DD82B3E_78D6_4999_8BEB_6F723F684B40__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcCommandDocumentAddDocument : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcCommandDocumentAddDocument(long time, const zString pid, const zString docName);

	virtual ~CLogpcCommandDocumentAddDocument();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcCommandDocumentAddDocument& rMsg);

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

	CLogpcCommandDocumentAddDocument(const CLogpcCommandDocumentAddDocument& oldMessage);
	CLogpcCommandDocumentAddDocument& operator=(const CLogpcCommandDocumentAddDocument& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;	// Target process' id
	const zString m_Name;	// Command document's name including the ".xml" suffix
};

#endif // !defined(AFX_LOGPCCOMMANDDOCUMENTADDDOCUMENT_H__3DD82B3E_78D6_4999_8BEB_6F723F684B40__INCLUDED_)
