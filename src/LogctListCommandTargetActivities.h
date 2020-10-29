// LogctListCommandTargetActivities.h: interface for the CLogctListCommandTargetActivities class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTLISTCOMMANDTARGETACTIVITIES_H__D831B9D2_20E1_446D_8560_3662E98F9871__INCLUDED_)
#define AFX_LOGCTLISTCOMMANDTARGETACTIVITIES_H__D831B9D2_20E1_446D_8560_3662E98F9871__INCLUDED_


#include "LogAnalysisMessage.h"

class CLogctListCommandTargetActivities : public CLogAnalysisMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctListCommandTargetActivities(long time, const zString label, const StringSequence& decList);

	virtual ~CLogctListCommandTargetActivities();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctListCommandTargetActivities& rMsg);

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

	CLogctListCommandTargetActivities(const CLogctListCommandTargetActivities& oldMessage);
	CLogctListCommandTargetActivities& operator=(const CLogctListCommandTargetActivities& rhs);


	// ****************************************
	// Data members
private:

    const zString m_TargetLabel; // Command target whose decorators are to be listed
    StringSequence  m_DecList;	 // Set of command target decorator labels

};

#endif // !defined(AFX_LOGCTLISTCOMMANDTARGETACTIVITIES_H__D831B9D2_20E1_446D_8560_3662E98F9871__INCLUDED_)
