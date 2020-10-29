// LogCommandDocumentImported.h: interface for the CLogCommandDocumentImported class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCOMMANDDOCUMENTIMPORTED_H__BE8619C3_18FA_4DDA_BB7F_F5DFE954F2AB__INCLUDED_)
#define AFX_LOGCOMMANDDOCUMENTIMPORTED_H__BE8619C3_18FA_4DDA_BB7F_F5DFE954F2AB__INCLUDED_


#include "LogInfoMessage.h"

class CLogCommandDocumentImported : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCommandDocumentImported(long time, const zString pid, const zString docName);

	virtual ~CLogCommandDocumentImported();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCommandDocumentImported& rMsg);

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

	CLogCommandDocumentImported(const CLogCommandDocumentImported& oldMessage);
	CLogCommandDocumentImported& operator=(const CLogCommandDocumentImported& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;	// Target process' id
	const zString m_Name;	// Command document's name including the ".xml" suffix
};


#endif // !defined(AFX_LOGCOMMANDDOCUMENTIMPORTED_H__BE8619C3_18FA_4DDA_BB7F_F5DFE954F2AB__INCLUDED_)
