// LogacEmptyBondACN.h: interface for the CLogacEmptyBondACN class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACEMPTYBONDACN_H__029F55AC_7CF3_45BD_9459_FF45709F156E__INCLUDED_)
#define AFX_LOGACEMPTYBONDACN_H__029F55AC_7CF3_45BD_9459_FF45709F156E__INCLUDED_


#include "LogWarningMessage.h"

class CLogacEmptyBondACN : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacEmptyBondACN(long time, const zString label);

	virtual ~CLogacEmptyBondACN();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacEmptyBondACN& rMsg);

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

	CLogacEmptyBondACN(const CLogacEmptyBondACN& oldMessage);
	CLogacEmptyBondACN& operator=(const CLogacEmptyBondACN& rhs);


	// ****************************************
	// Data members
private:

	const zString m_ACNLabel;	// Active cell network's name
};

#endif // !defined(AFX_LOGACEMPTYBONDACN_H__029F55AC_7CF3_45BD_9459_FF45709F156E__INCLUDED_)
