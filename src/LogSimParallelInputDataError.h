// LogSimParallelInputDataError.h: interface for the CLogSimParallelInputDataError class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSIMPARALLELINPUTDATAERROR_H__DCFB8342_D7FC_40A6_8402_88A7C99297D5__INCLUDED_)
#define AFX_LOGSIMPARALLELINPUTDATAERROR_H__DCFB8342_D7FC_40A6_8402_88A7C99297D5__INCLUDED_


#include "LogErrorMessage.h"

class CLogSimParallelInputDataError : public CLogErrorMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSimParallelInputDataError(long time);

	virtual ~CLogSimParallelInputDataError();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSimParallelInputDataError& rMsg);

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

	CLogSimParallelInputDataError(const CLogSimParallelInputDataError& oldMessage);
	CLogSimParallelInputDataError& operator=(const CLogSimParallelInputDataError& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGSIMPARALLELINPUTDATAERROR_H__DCFB8342_D7FC_40A6_8402_88A7C99297D5__INCLUDED_)
