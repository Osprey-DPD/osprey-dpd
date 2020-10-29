// LogSimISError.h: interface for the CLogSimISError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSIMISERROR_H__CB135509_97AA_46AC_8F0E_448E4A118583__INCLUDED_)
#define AFX_LOGSIMISERROR_H__CB135509_97AA_46AC_8F0E_448E4A118583__INCLUDED_


#include "LogErrorMessage.h"

class CLogSimISError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSimISError(long time);

	virtual ~CLogSimISError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSimISError& rMsg);

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
	// by declaring them privately but providing NO definitions.

	CLogSimISError(const CLogSimISError& oldMessage);
	CLogSimISError& operator=(const CLogSimISError& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGSIMISERROR_H__CB135509_97AA_46AC_8F0E_448E4A118583__INCLUDED_)
