// LogpcProcessNotModifiable.h: interface for the CLogpcProcessNotModifiable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCPROCESSNOTMODIFIABLE_H__B8FFEC5C_76DB_4683_A3A1_BB5BB6050615__INCLUDED_)
#define AFX_LOGPCPROCESSNOTMODIFIABLE_H__B8FFEC5C_76DB_4683_A3A1_BB5BB6050615__INCLUDED_


#include "LogWarningMessage.h"

class CLogpcProcessNotModifiable : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcProcessNotModifiable(long time, const zString pid);

	virtual ~CLogpcProcessNotModifiable();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcProcessNotModifiable& rMsg);

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

	CLogpcProcessNotModifiable(const CLogpcProcessNotModifiable& oldMessage);
	CLogpcProcessNotModifiable& operator=(const CLogpcProcessNotModifiable& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;			// Target process' id

};

#endif // !defined(AFX_LOGPCPROCESSNOTMODIFIABLE_H__B8FFEC5C_76DB_4683_A3A1_BB5BB6050615__INCLUDED_)
