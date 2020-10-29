// LogctSetBondStrength.h: interface for the CLogctSetBondStrength class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTSETBONDSTRENGTH_H__F8FFB25F_8F78_42F9_A292_54A40EE2043E__INCLUDED_)
#define AFX_LOGCTSETBONDSTRENGTH_H__F8FFB25F_8F78_42F9_A292_54A40EE2043E__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctSetBondStrength : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctSetBondStrength(long time, const zString cmdTarget, 
					      const zString name, long type, long total, 
					      double spring, double length);

	virtual ~CLogctSetBondStrength();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctSetBondStrength& rMsg);

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

	CLogctSetBondStrength(const CLogctSetBondStrength& oldMessage);
	CLogctSetBondStrength& operator=(const CLogctSetBondStrength& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_Name;
	const long      m_Type;
	const long      m_Total;
	const double	m_SpringConstant;
	const double	m_UnstretchedLength;
};

#endif // !defined(AFX_LOGCTSETBONDSTRENGTH_H__F8FFB25F_8F78_42F9_A292_54A40EE2043E__INCLUDED_)
