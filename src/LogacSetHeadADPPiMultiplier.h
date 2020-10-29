// LogacSetHeadADPPiMultiplier.h: interface for the CLogacSetHeadADPPiMultiplier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETHEADADPPIMULTIPLIER_H__52CDB17A_9871_4448_AEC0_5B5658139674__INCLUDED_)
#define AFX_LOGACSETHEADADPPIMULTIPLIER_H__52CDB17A_9871_4448_AEC0_5B5658139674__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetHeadADPPiMultiplier : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetHeadADPPiMultiplier(long time, const zString type, double factor);

	virtual ~CLogacSetHeadADPPiMultiplier();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetHeadADPPiMultiplier& rMsg);

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

	CLogacSetHeadADPPiMultiplier(const CLogacSetHeadADPPiMultiplier& oldMessage);
	CLogacSetHeadADPPiMultiplier& operator=(const CLogacSetHeadADPPiMultiplier& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	    // Type of active cell network modified
	const double	m_Multiplier;	// Multiplier of the transition's basal probability
};

#endif // !defined(AFX_LOGACSETHEADADPPIMULTIPLIER_H__52CDB17A_9871_4448_AEC0_5B5658139674__INCLUDED_)
