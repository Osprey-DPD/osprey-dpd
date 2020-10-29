// LogctCompositeCreated.h: interface for the CLogctCompositeCreated class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCOMPOSITECREATED_H__A7D90D5F_2B81_4374_BD30_D50CFE0B5FF0__INCLUDED_)
#define AFX_LOGCTCOMPOSITECREATED_H__A7D90D5F_2B81_4374_BD30_D50CFE0B5FF0__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctCompositeCreated : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctCompositeCreated(long time, const zString label);

	virtual ~CLogctCompositeCreated();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctCompositeCreated& rMsg);

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

	CLogctCompositeCreated(const CLogctCompositeCreated& oldMessage);
	CLogctCompositeCreated& operator=(const CLogctCompositeCreated& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Label;	// Command target's name
};

#endif // !defined(AFX_LOGCTCOMPOSITECREATED_H__A7D90D5F_2B81_4374_BD30_D50CFE0B5FF0__INCLUDED_)
