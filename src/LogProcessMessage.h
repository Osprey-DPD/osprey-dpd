// LogProcessMessage.h: interface for the CLogProcessMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPROCESSMESSAGE_H__4B667A77_E6BE_4224_9FB5_8E59181CE2C1__INCLUDED_)
#define AFX_LOGPROCESSMESSAGE_H__4B667A77_E6BE_4224_9FB5_8E59181CE2C1__INCLUDED_


#include "LogMessage.h"

class CLogProcessMessage : public CLogMessage  
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogProcessMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:


	friend zOutStream& operator<<(zOutStream& os, const CLogProcessMessage& rMsg);

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

	CLogProcessMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogProcessMessage(const CLogProcessMessage& oldMessage);
	CLogProcessMessage& operator=(const CLogProcessMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGPROCESSMESSAGE_H__4B667A77_E6BE_4224_9FB5_8E59181CE2C1__INCLUDED_)
