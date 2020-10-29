// LogErrorMessage.h: interface for the CLogErrorMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGERRORMESSAGE_H__936B7AEE_227D_4E2F_B990_1BC147958B99__INCLUDED_)
#define AFX_LOGERRORMESSAGE_H__936B7AEE_227D_4E2F_B990_1BC147958B99__INCLUDED_


#include "LogMessage.h"

class CLogErrorMessage : public CLogMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogErrorMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogErrorMessage& rMsg);

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

	CLogErrorMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogErrorMessage(const CLogErrorMessage& oldMessage);
	CLogErrorMessage& operator=(const CLogErrorMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGERRORMESSAGE_H__936B7AEE_227D_4E2F_B990_1BC147958B99__INCLUDED_)
