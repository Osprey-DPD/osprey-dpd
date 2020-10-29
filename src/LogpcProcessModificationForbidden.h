// LogpcProcessModificationForbidden.h: interface for the CLogpcProcessModificationForbidden class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCPROCESSMODIFICATIONFORBIDDEN_H__93290EF7_154A_4AE1_979C_C317078E3799__INCLUDED_)
#define AFX_LOGPCPROCESSMODIFICATIONFORBIDDEN_H__93290EF7_154A_4AE1_979C_C317078E3799__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcProcessModificationForbidden : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcProcessModificationForbidden(long time, const zString pid);

	virtual ~CLogpcProcessModificationForbidden();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcProcessModificationForbidden& rMsg);

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

	CLogpcProcessModificationForbidden(const CLogpcProcessModificationForbidden& oldMessage);
	CLogpcProcessModificationForbidden& operator=(const CLogpcProcessModificationForbidden& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;			// Target process' id

};

#endif // !defined(AFX_LOGPCPROCESSMODIFICATIONFORBIDDEN_H__93290EF7_154A_4AE1_979C_C317078E3799__INCLUDED_)
