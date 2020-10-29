// LogpcCommandDocumentGetPendingDocs.h: interface for the CLogpcCommandDocumentGetPendingDocs class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCCOMMANDDOCUMENTGETPENDINGDOCS_H__17600A8E_68CC_48B3_A40E_E3C552B0F54B__INCLUDED_)
#define AFX_LOGPCCOMMANDDOCUMENTGETPENDINGDOCS_H__17600A8E_68CC_48B3_A40E_E3C552B0F54B__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcCommandDocumentGetPendingDocs : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcCommandDocumentGetPendingDocs(long time, const zString pid, const StringSequence docNames);

	virtual ~CLogpcCommandDocumentGetPendingDocs();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcCommandDocumentGetPendingDocs& rMsg);

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

	CLogpcCommandDocumentGetPendingDocs(const CLogpcCommandDocumentGetPendingDocs& oldMessage);
	CLogpcCommandDocumentGetPendingDocs& operator=(const CLogpcCommandDocumentGetPendingDocs& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;					// Target process' id
	const StringSequence m_DocumentNames;	// Names of pending command documents
};

#endif // !defined(AFX_LOGPCCOMMANDDOCUMENTGETPENDINGDOCS_H__17600A8E_68CC_48B3_A40E_E3C552B0F54B__INCLUDED_)
