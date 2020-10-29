// LogacEmptyPolymerACN.h: interface for the CLogacEmptyPolymerACN class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACEMPTYPOLYMERACN_H__A7A5EC18_739A_4488_8253_7271B8568925__INCLUDED_)
#define AFX_LOGACEMPTYPOLYMERACN_H__A7A5EC18_739A_4488_8253_7271B8568925__INCLUDED_


#include "LogWarningMessage.h"

class CLogacEmptyPolymerACN : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacEmptyPolymerACN(long time, const zString label);

	virtual ~CLogacEmptyPolymerACN();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacEmptyPolymerACN& rMsg);

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

	CLogacEmptyPolymerACN(const CLogacEmptyPolymerACN& oldMessage);
	CLogacEmptyPolymerACN& operator=(const CLogacEmptyPolymerACN& rhs);


	// ****************************************
	// Data members
private:

	const zString m_ACNLabel;	// Active cell network's name
};

#endif // !defined(AFX_LOGACEMPTYPOLYMERACN_H__A7A5EC18_739A_4488_8253_7271B8568925__INCLUDED_)
