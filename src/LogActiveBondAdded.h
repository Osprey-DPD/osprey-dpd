// LogActiveBondAdded.h: interface for the CLogActiveBondAdded class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACTIVEBONDADDED_H__4C806411_D29A_42E1_8431_E6B670A0C99D__INCLUDED_)
#define AFX_LOGACTIVEBONDADDED_H__4C806411_D29A_42E1_8431_E6B670A0C99D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogActiveBondAdded : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogActiveBondAdded(long time, const zString bondName, const zString name1, const zString name2, 
														   long type1, long type2);

	virtual ~CLogActiveBondAdded();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogActiveBondAdded& rMsg);

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

	CLogActiveBondAdded(const CLogActiveBondAdded& oldMessage);
	CLogActiveBondAdded& operator=(const CLogActiveBondAdded& rhs);


	// ****************************************
	// Data members
private:

	const zString m_BondName;			// Name of active bond type
	const zString m_HeadMonomerName;	// Name of polymers bound to this active bond
	const zString m_TailMonomerName;	// Name of polymers wrapped by this active bond
	         long m_HeadMonomerType;
	         long m_TailMonomerType;
};

#endif // !defined(AFX_LOGACTIVEBONDADDED_H__4C806411_D29A_42E1_8431_E6B670A0C99D__INCLUDED_)
