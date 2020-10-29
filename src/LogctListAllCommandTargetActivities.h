// LogctListAllCommandTargetActivities.h: interface for the CLogctListAllCommandTargetActivities class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTLISTALLCOMMANDTARGETACTIVITIES_H__FA201EFE_DE79_4B6A_9079_7CA30BBD9432__INCLUDED_)
#define AFX_LOGCTLISTALLCOMMANDTARGETACTIVITIES_H__FA201EFE_DE79_4B6A_9079_7CA30BBD9432__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctListAllCommandTargetActivities : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctListAllCommandTargetActivities(long time, const zArray2dString& decList);

	virtual ~CLogctListAllCommandTargetActivities();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctListAllCommandTargetActivities& rMsg);

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

	CLogctListAllCommandTargetActivities(const CLogctListAllCommandTargetActivities& oldMessage);
	CLogctListAllCommandTargetActivities& operator=(const CLogctListAllCommandTargetActivities& rhs);


	// ****************************************
	// Data members
private:

    zArray2dString  m_DecList;	    // Set of all command target decorators

    long m_TargetTotal;             // Number of command targets

};

#endif // !defined(AFX_LOGCTLISTALLCOMMANDTARGETACTIVITIES_H__FA201EFE_DE79_4B6A_9079_7CA30BBD9432__INCLUDED_)
