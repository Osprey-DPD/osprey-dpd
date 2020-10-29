// LogActiveSimBoxCreated.h: interface for the CLogActiveSimBoxCreated class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACTIVESIMBOXCREATED_H__A227AEE5_44EC_47C4_8311_B10D025A595B__INCLUDED_)
#define AFX_LOGACTIVESIMBOXCREATED_H__A227AEE5_44EC_47C4_8311_B10D025A595B__INCLUDED_


#include "LogConstraintMessage.h"

class CLogActiveSimBoxCreated : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogActiveSimBoxCreated(long time);

	virtual ~CLogActiveSimBoxCreated();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogActiveSimBoxCreated& rMsg);

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

	CLogActiveSimBoxCreated(const CLogActiveSimBoxCreated& oldMessage);
	CLogActiveSimBoxCreated& operator=(const CLogActiveSimBoxCreated& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGACTIVESIMBOXCREATED_H__A227AEE5_44EC_47C4_8311_B10D025A595B__INCLUDED_)
