// LogacCancelConstantFreeActiveBondConcentration.h: interface for the CLogacCancelConstantFreeActiveBondConcentration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACCANCELCONSTANTFREEACTIVEBONDCONCENTRATION_H__A3E38999_8E8B_4F5F_A1A3_7F478AC50B2D__INCLUDED_)
#define AFX_LOGACCANCELCONSTANTFREEACTIVEBONDCONCENTRATION_H__A3E38999_8E8B_4F5F_A1A3_7F478AC50B2D__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacCancelConstantFreeActiveBondConcentration : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacCancelConstantFreeActiveBondConcentration(long time, const zString type);

	virtual ~CLogacCancelConstantFreeActiveBondConcentration();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacCancelConstantFreeActiveBondConcentration& rMsg);

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

	CLogacCancelConstantFreeActiveBondConcentration(const CLogacCancelConstantFreeActiveBondConcentration& oldMessage);
	CLogacCancelConstantFreeActiveBondConcentration& operator=(const CLogacCancelConstantFreeActiveBondConcentration& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		    // Type of active cell network modified
};

#endif // !defined(AFX_LOGACCANCELCONSTANTFREEACTIVEBONDCONCENTRATION_H__A3E38999_8E8B_4F5F_A1A3_7F478AC50B2D__INCLUDED_)
