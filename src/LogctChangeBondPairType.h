// LogctChangeBondPairType.h: interface for the CLogctChangeBondPairType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTCHANGEBONDPAIRTYPE_H__B6C0ECAB_CC6C_49E4_94FA_38F0AB343D03__INCLUDED_)
#define AFX_LOGCTCHANGEBONDPAIRTYPE_H__B6C0ECAB_CC6C_49E4_94FA_38F0AB343D03__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctChangeBondPairType : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctChangeBondPairType(long time, const zString cmdTarget, 
					   const zString oldName, long oldType, long newType);

	virtual ~CLogctChangeBondPairType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctChangeBondPairType& rMsg);

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

	CLogctChangeBondPairType(const CLogctChangeBondPairType& oldMessage);
	CLogctChangeBondPairType& operator=(const CLogctChangeBondPairType& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_OldName;
	const long      m_OldType;
	const long      m_NewType;
};

#endif // !defined(AFX_LOGCTCHANGEBONDPAIRTYPE_H__B6C0ECAB_CC6C_49E4_94FA_38F0AB343D03__INCLUDED_)
