// LogActiveSimBoxDeleted.h: interface for the CLogActiveSimBoxDeleted class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACTIVESIMBOXDELETED_H__577EA2D9_51A6_4ED9_8180_DC7BF1134C91__INCLUDED_)
#define AFX_LOGACTIVESIMBOXDELETED_H__577EA2D9_51A6_4ED9_8180_DC7BF1134C91__INCLUDED_


#include "LogConstraintMessage.h"

class CLogActiveSimBoxDeleted : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogActiveSimBoxDeleted(long time);

	virtual ~CLogActiveSimBoxDeleted();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogActiveSimBoxDeleted& rMsg);

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

	CLogActiveSimBoxDeleted(const CLogActiveSimBoxDeleted& oldMessage);
	CLogActiveSimBoxDeleted& operator=(const CLogActiveSimBoxDeleted& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGACTIVESIMBOXDELETED_H__577EA2D9_51A6_4ED9_8180_DC7BF1134C91__INCLUDED_)
