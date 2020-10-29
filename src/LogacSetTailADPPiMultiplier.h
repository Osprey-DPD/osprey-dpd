// LogacSetTailADPPiMultiplier.h: interface for the CLogacSetTailADPPiMultiplier class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETTAILADPPIMULTIPLIER_H__5E01CA56_C184_44F8_8379_3B681B6CC81E__INCLUDED_)
#define AFX_LOGACSETTAILADPPIMULTIPLIER_H__5E01CA56_C184_44F8_8379_3B681B6CC81E__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacSetTailADPPiMultiplier : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetTailADPPiMultiplier(long time, const zString type, double factor);

	virtual ~CLogacSetTailADPPiMultiplier();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetTailADPPiMultiplier& rMsg);

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

	CLogacSetTailADPPiMultiplier(const CLogacSetTailADPPiMultiplier& oldMessage);
	CLogacSetTailADPPiMultiplier& operator=(const CLogacSetTailADPPiMultiplier& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;	    // Type of active cell network modified
	const double	m_Multiplier;	// Multiplier of the transition's basal probability
};

#endif // !defined(AFX_LOGACSETTAILADPPIMULTIPLIER_H__5E01CA56_C184_44F8_8379_3B681B6CC81E__INCLUDED_)
