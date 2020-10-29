// LogacInactivateForminNucleation.h: interface for the CLogacInactivateForminNucleation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACINACTIVATEFORMINNUCLEATION_H__40F8E359_ACC2_401D_9516_B1A0EA326DE3__INCLUDED_)
#define AFX_LOGACINACTIVATEFORMINNUCLEATION_H__40F8E359_ACC2_401D_9516_B1A0EA326DE3__INCLUDED_


#include "LogInfoMessage.h"

class CLogacInactivateForminNucleation : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacInactivateForminNucleation(long time, const zString type);

	virtual ~CLogacInactivateForminNucleation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacInactivateForminNucleation& rMsg);

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

	CLogacInactivateForminNucleation(const CLogacInactivateForminNucleation& oldMessage);
	CLogacInactivateForminNucleation& operator=(const CLogacInactivateForminNucleation& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACINACTIVATEFORMINNUCLEATION_H__40F8E359_ACC2_401D_9516_B1A0EA326DE3__INCLUDED_)
