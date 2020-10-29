// LogacSetHeadADPMultiplier.h: interface for the CLogacSetHeadADPMultiplier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETHEADADPMULTIPLIER_H__E847B52E_93B4_44E4_8642_B2FAFCC30CB4__INCLUDED_)
#define AFX_LOGACSETHEADADPMULTIPLIER_H__E847B52E_93B4_44E4_8642_B2FAFCC30CB4__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetHeadADPMultiplier : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetHeadADPMultiplier(long time, const zString type, double factor);

	virtual ~CLogacSetHeadADPMultiplier();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetHeadADPMultiplier& rMsg);

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

	CLogacSetHeadADPMultiplier(const CLogacSetHeadADPMultiplier& oldMessage);
	CLogacSetHeadADPMultiplier& operator=(const CLogacSetHeadADPMultiplier& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	    // Type of active cell network modified
	const double	m_Multiplier;	// Multiplier of the transition's basal probability
};

#endif // !defined(AFX_LOGACSETHEADADPMULTIPLIER_H__E847B52E_93B4_44E4_8642_B2FAFCC30CB4__INCLUDED_)
