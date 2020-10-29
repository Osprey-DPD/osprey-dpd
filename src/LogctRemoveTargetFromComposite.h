// LogctRemoveTargetFromComposite.h: interface for the CLogctRemoveTargetFromComposite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTREMOVETARGETFROMCOMPOSITE_H__B567E5B6_6D48_4487_8D8B_9B59B0F22D92__INCLUDED_)
#define AFX_LOGCTREMOVETARGETFROMCOMPOSITE_H__B567E5B6_6D48_4487_8D8B_9B59B0F22D92__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctRemoveTargetFromComposite : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctRemoveTargetFromComposite(long time, const zString destlabel, 
									const zString sourcelabel, long total);

	virtual ~CLogctRemoveTargetFromComposite();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctRemoveTargetFromComposite& rMsg);

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

	CLogctRemoveTargetFromComposite(const CLogctRemoveTargetFromComposite& oldMessage);
	CLogctRemoveTargetFromComposite& operator=(const CLogctRemoveTargetFromComposite& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Destination;	// Composite command target's name
	const zString m_Source;			// Name of target to remove from composite
	const long    m_Total;			// Targets held in composite target
};

#endif // !defined(AFX_LOGCTREMOVETARGETFROMCOMPOSITE_H__B567E5B6_6D48_4487_8D8B_9B59B0F22D92__INCLUDED_)
