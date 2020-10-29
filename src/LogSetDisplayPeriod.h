// LogSetDisplayPeriod.h: interface for the CLogSetDisplayPeriod class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETDISPLAYPERIOD_H__B218A7FC_8534_401F_A6DF_AC1D3884202D__INCLUDED_)
#define AFX_LOGSETDISPLAYPERIOD_H__B218A7FC_8534_401F_A6DF_AC1D3884202D__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetDisplayPeriod : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetDisplayPeriod(long time, long newPeriod);

	virtual ~CLogSetDisplayPeriod();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetDisplayPeriod& rMsg);

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

	CLogSetDisplayPeriod(const CLogSetDisplayPeriod& oldMessage);
	CLogSetDisplayPeriod& operator=(const CLogSetDisplayPeriod& rhs);


	// ****************************************
	// Data members
private:

	const long m_Period; // Number of timesteps between current state output
};

#endif // !defined(AFX_LOGSETDISPLAYPERIOD_H__B218A7FC_8534_401F_A6DF_AC1D3884202D__INCLUDED_)
