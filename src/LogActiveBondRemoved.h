// LogActiveBondRemoved.h: interface for the CLogActiveBondRemoved class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACTIVEBONDREMOVED_H__C22C2958_9C18_434B_97F8_BB1A82868FE4__INCLUDED_)
#define AFX_LOGACTIVEBONDREMOVED_H__C22C2958_9C18_434B_97F8_BB1A82868FE4__INCLUDED_


#include "LogConstraintMessage.h"

class CLogActiveBondRemoved : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogActiveBondRemoved(long time, const zString bondName, const zString name1, const zString name2, 
														     long type1, long type2);

	virtual ~CLogActiveBondRemoved();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogActiveBondRemoved& rMsg);

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

	CLogActiveBondRemoved(const CLogActiveBondRemoved& oldMessage);
	CLogActiveBondRemoved& operator=(const CLogActiveBondRemoved& rhs);


	// ****************************************
	// Data members
private:

	const zString m_BondName;		// Name of active bond type
	const zString m_HeadMonomerName;	// Name of polymers bound to this active bond
	const zString m_TailMonomerName;	// Name of polymers wrapped by this active bond
	         long m_HeadMonomerType;
	         long m_TailMonomerType;
};

#endif // !defined(AFX_LOGACTIVEBONDREMOVED_H__C22C2958_9C18_434B_97F8_BB1A82868FE4__INCLUDED_)
