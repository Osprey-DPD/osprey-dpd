// LogacColourBondsByATPState.h: interface for the CLogacColourBondsByATPState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACCOLOURBONDSBYATPSTATE_H__6223B213_828B_4F43_B4A4_E9842AB84283__INCLUDED_)
#define AFX_LOGACCOLOURBONDSBYATPSTATE_H__6223B213_828B_4F43_B4A4_E9842AB84283__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacColourBondsByATPState : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacColourBondsByATPState(long time, const zString type, long ATPId, long ADPPiId, long ADPId);

	virtual ~CLogacColourBondsByATPState();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacColourBondsByATPState& rMsg);

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

	CLogacColourBondsByATPState(const CLogacColourBondsByATPState& oldMessage);
	CLogacColourBondsByATPState& operator=(const CLogacColourBondsByATPState& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	       // Type of active cell network modified
	const long	    m_ATPDisplayId;    // Display Id for ATP state
	const long	    m_ADPPiDisplayId;  // Display Id for ADP-Pi state
	const long	    m_ADPDisplayId;    // Display Id for ADP state

};

#endif // !defined(AFX_LOGACCOLOURBONDSBYATPSTATE_H__6223B213_828B_4F43_B4A4_E9842AB84283__INCLUDED_)
