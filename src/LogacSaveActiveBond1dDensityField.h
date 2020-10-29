// LogacSaveActiveBond1dDensityField.h: interface for the CLogacSaveActiveBond1dDensityField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSAVEACTIVEBOND1DDENSITYFIELD_H__F43276CC_E151_460F_BD3B_8FD2CE5875AA__INCLUDED_)
#define AFX_LOGACSAVEACTIVEBOND1DDENSITYFIELD_H__F43276CC_E151_460F_BD3B_8FD2CE5875AA__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSaveActiveBond1dDensityField : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSaveActiveBond1dDensityField(long time, const zString type);

	virtual ~CLogacSaveActiveBond1dDensityField();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSaveActiveBond1dDensityField& rMsg);

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

	CLogacSaveActiveBond1dDensityField(const CLogacSaveActiveBond1dDensityField& oldMessage);
	CLogacSaveActiveBond1dDensityField& operator=(const CLogacSaveActiveBond1dDensityField& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACSAVEACTIVEBOND1DDENSITYFIELD_H__F43276CC_E151_460F_BD3B_8FD2CE5875AA__INCLUDED_)
