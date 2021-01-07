// LogWarningMessage.h: interface for the CLogWarningMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGWARNINGMESSAGE_H__4F6924EB_4B48_456D_AFC1_99A60DAB2A83__INCLUDED_)
#define AFX_LOGWARNINGMESSAGE_H__4F6924EB_4B48_456D_AFC1_99A60DAB2A83__INCLUDED_



#include "LogMessage.h"

class CLogWarningMessage : public CLogMessage
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogWarningMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogWarningMessage& rMsg);

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

	CLogWarningMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogWarningMessage(const CLogWarningMessage& oldMessage);
	CLogWarningMessage& operator=(const CLogWarningMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGWARNINGMESSAGE_H__4F6924EB_4B48_456D_AFC1_99A60DAB2A83__INCLUDED_)



