// LogpcCommandDocumentGetPendingDocTotal.h: interface for the CLogpcCommandDocumentGetPendingDocTotal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCCOMMANDDOCUMENTGETPENDINGDOCTOTAL_H__F46FE6F5_88F2_4053_9295_8B5ADEB831BA__INCLUDED_)
#define AFX_LOGPCCOMMANDDOCUMENTGETPENDINGDOCTOTAL_H__F46FE6F5_88F2_4053_9295_8B5ADEB831BA__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcCommandDocumentGetPendingDocTotal : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcCommandDocumentGetPendingDocTotal(long time, const zString pid, long docTotal);

	virtual ~CLogpcCommandDocumentGetPendingDocTotal();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcCommandDocumentGetPendingDocTotal& rMsg);

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

	CLogpcCommandDocumentGetPendingDocTotal(const CLogpcCommandDocumentGetPendingDocTotal& oldMessage);
	CLogpcCommandDocumentGetPendingDocTotal& operator=(const CLogpcCommandDocumentGetPendingDocTotal& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;					// Target process' id
	const long m_DocumentTotal;				// Number of pending command documents
};

#endif // !defined(AFX_LOGPCCOMMANDDOCUMENTGETPENDINGDOCTOTAL_H__F46FE6F5_88F2_4053_9295_8B5ADEB831BA__INCLUDED_)
