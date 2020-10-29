// LogacInvalidEventCommandTarget.h: interface for the CLogacInvalidEventCommandTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACINVALIDEVENTCOMMANDTARGET_H__7467D395_E287_4855_9BC3_E4C1F7739CAD__INCLUDED_)
#define AFX_LOGACINVALIDEVENTCOMMANDTARGET_H__7467D395_E287_4855_9BC3_E4C1F7739CAD__INCLUDED_


#include "LogWarningMessage.h"

class CLogacInvalidEventCommandTarget : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacInvalidEventCommandTarget(long time, const zString type, 
		                            const zString actualType,
		                            const zString expectedType);

	virtual ~CLogacInvalidEventCommandTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacInvalidEventCommandTarget& rMsg);

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

	CLogacInvalidEventCommandTarget(const CLogacInvalidEventCommandTarget& oldMessage);
	CLogacInvalidEventCommandTarget& operator=(const CLogacInvalidEventCommandTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Type;				// Target Active Cell Network
	const zString m_ActualEventType;	// Event type implemented by the ACN
	const zString m_ExpectedEventType;	// Event type expected by the command

};

#endif // !defined(AFX_LOGACINVALIDEVENTCOMMANDTARGET_H__7467D395_E287_4855_9BC3_E4C1F7739CAD__INCLUDED_)
