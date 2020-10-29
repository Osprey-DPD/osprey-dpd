// prProbeFieldProcess.h: interface for the prProbeFieldProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRPROBEFIELDPROCESS_H__1F46FE7C_C40A_41F2_A909_AD72F26D08E6__INCLUDED_)
#define AFX_PRPROBEFIELDPROCESS_H__1F46FE7C_C40A_41F2_A909_AD72F26D08E6__INCLUDED_


#include "xxProcess.h"

class prProbeFieldProcess : public xxProcess
{
	// ****************************************
	// Construction/Destruction

	// Protected constructor prevents instantiation of this class
public:

	virtual ~prProbeFieldProcess();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// Protected local functions
protected:

	prProbeFieldProcess(bool bValid);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// ****************************************
	// Data members

private:


};

#endif // !defined(AFX_PRPROBEFIELDPROCESS_H__1F46FE7C_C40A_41F2_A909_AD72F26D08E6__INCLUDED_)
