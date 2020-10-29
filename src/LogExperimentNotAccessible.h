// LogExperimentNotAccessible.h: interface for the CLogExperimentNotAccessible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGEXPERIMENTNOTACCESSIBLE_H__E264731E_6249_4DB0_90B0_6E5D9B4F87F0__INCLUDED_)
#define AFX_LOGEXPERIMENTNOTACCESSIBLE_H__E264731E_6249_4DB0_90B0_6E5D9B4F87F0__INCLUDED_


#include "LogWarningMessage.h"

class CLogExperimentNotAccessible : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogExperimentNotAccessible(long time);

	virtual ~CLogExperimentNotAccessible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogExperimentNotAccessible& rMsg);

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Public access functions
public:

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

	CLogExperimentNotAccessible(const CLogExperimentNotAccessible& oldMessage);
	CLogExperimentNotAccessible& operator=(const CLogExperimentNotAccessible& rhs);


	// ****************************************
	// Data members
private:


};

#endif // !defined(AFX_LOGEXPERIMENTNOTACCESSIBLE_H__E264731E_6249_4DB0_90B0_6E5D9B4F87F0__INCLUDED_)
