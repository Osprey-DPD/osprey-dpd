// LogctSetBondStiffness.h: interface for the CLogctSetBondStiffness class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTSETBONDSTIFFNESS_H__030ffb68_6f6a_42e8_815e_949d78df7faf__INCLUDED_)
#define AFX_LOGCTSETBONDSTIFFNESS_H__030ffb68_6f6a_42e8_815e_949d78df7faf__INCLUDED_


#include "LogConstraintMessage.h"

class CLogctSetBondStiffness : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctSetBondStiffness(long time, const zString cmdTarget, 
					      const zString name, long type, long total, 
					      double modulus, double phi0);

	virtual ~CLogctSetBondStiffness();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctSetBondStiffness& rMsg);

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

	CLogctSetBondStiffness(const CLogctSetBondStiffness& oldMessage);
	CLogctSetBondStiffness& operator=(const CLogctSetBondStiffness& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_CmdTarget;	// Command target's name
	const zString	m_Name;
	const long      m_Type;
	const long      m_Total;
	const double	m_BendingModulus;
	const double	m_Phi0;
};

#endif // !defined(AFX_LOGCTSETBONDSTIFFNESS_H__030ffb68_6f6a_42e8_815e_949d78df7faf__INCLUDED_)
