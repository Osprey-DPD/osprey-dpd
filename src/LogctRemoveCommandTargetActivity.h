// LogctRemoveCommandTargetActivity.h: interface for the CLogctRemoveCommandTargetActivity class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTREMOVECOMMANDTARGETACTIVITY_H__1EEEC6CE_A9F3_4D51_824E_CFEE8AB17D67__INCLUDED_)
#define AFX_LOGCTREMOVECOMMANDTARGETACTIVITY_H__1EEEC6CE_A9F3_4D51_824E_CFEE8AB17D67__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctRemoveCommandTargetActivity : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctRemoveCommandTargetActivity(long time, const zString target,
									  const zString activity, long activitiesLeft);

	virtual ~CLogctRemoveCommandTargetActivity();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRemoveCommandTargetActivity& rMsg);

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

	CLogctRemoveCommandTargetActivity(const CLogctRemoveCommandTargetActivity& oldMessage);
	CLogctRemoveCommandTargetActivity& operator=(const CLogctRemoveCommandTargetActivity& rhs);


	// ****************************************
	// Data members
private:

	const zString m_TargetLabel;	// Active command target label
	const zString m_ActivityLabel;	// Decorator instance label
	const long    m_ActivitiesLeft;	// Number of decorators left wrapping target
};

#endif // !defined(AFX_LOGCTREMOVECOMMANDTARGETACTIVITY_H__1EEEC6CE_A9F3_4D51_824E_CFEE8AB17D67__INCLUDED_)
