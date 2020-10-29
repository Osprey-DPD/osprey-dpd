// LogSetCurrentStateDefaultFormat.h: interface for the CLogSetCurrentStateDefaultFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETCURRENTSTATEDEFAULTFORMAT_H__82C834C5_2F11_4900_B87D_2FE003A21CAB__INCLUDED_)
#define AFX_LOGSETCURRENTSTATEDEFAULTFORMAT_H__82C834C5_2F11_4900_B87D_2FE003A21CAB__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetCurrentStateDefaultFormat : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetCurrentStateDefaultFormat(long time, const zString newFormat);

	virtual ~CLogSetCurrentStateDefaultFormat();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetCurrentStateDefaultFormat& rMsg);

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

	CLogSetCurrentStateDefaultFormat(const CLogSetCurrentStateDefaultFormat& oldMessage);
	CLogSetCurrentStateDefaultFormat& operator=(const CLogSetCurrentStateDefaultFormat& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Format; // Current state format identifying string, e.g., Povray, Amira.
};

#endif // !defined(AFX_LOGSETCURRENTSTATEDEFAULTFORMAT_H__82C834C5_2F11_4900_B87D_2FE003A21CAB__INCLUDED_)
