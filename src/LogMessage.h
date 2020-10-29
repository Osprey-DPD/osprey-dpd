// LogMessage.h: interface for the CLogMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGMESSAGE_H__0D23FEF8_D4AF_4E6A_A887_F0E084BB1A4D__INCLUDED_)
#define AFX_LOGMESSAGE_H__0D23FEF8_D4AF_4E6A_A887_F0E084BB1A4D__INCLUDED_


#include "xxMessage.h"

class CLogMessage : public xxMessage
{
	// ****************************************
	// Construction/Destruction: protected constructor declared below
public:

	// Protected constructor and PVF Serialize() prevent instances of CLogMessage
	// being created. Each sub-class' constructor must pass the creation time
	// of a message to this base class.

	virtual ~CLogMessage();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const = 0;

	// ****************************************
	// Protected local functions
protected:

	CLogMessage(long time);


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	CLogMessage(const CLogMessage& oldMessage);
	CLogMessage& operator=(const CLogMessage& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGMESSAGE_H__0D23FEF8_D4AF_4E6A_A887_F0E084BB1A4D__INCLUDED_)
