// LogBuilderError.h: interface for the CLogBuilderError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGBUILDERERROR_H__C3C8BE0D_5CC2_4099_BD6A_50A907DAB5C5__INCLUDED_)
#define AFX_LOGBUILDERERROR_H__C3C8BE0D_5CC2_4099_BD6A_50A907DAB5C5__INCLUDED_


#include "LogErrorMessage.h"

class CLogBuilderError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogBuilderError(long time, const zString message);

	virtual ~CLogBuilderError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogBuilderError& rMsg);

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
	// by declaring them privately but providing NO definitions.

	CLogBuilderError(const CLogBuilderError& oldMessage);
	CLogBuilderError& operator=(const CLogBuilderError& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Message;	// Message describing the error condition

};
#endif // !defined(AFX_LOGBUILDERERROR_H__C3C8BE0D_5CC2_4099_BD6A_50A907DAB5C5__INCLUDED_)
