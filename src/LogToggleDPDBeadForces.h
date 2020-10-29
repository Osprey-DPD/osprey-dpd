// LogToggleDPDBeadForces.h: interface for the CLogToggleDPDBeadForces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLEDPDBEADFORCES_H__4D38CB70_23A3_4C6C_8E13_DAB7376E3578__INCLUDED_)
#define AFX_LOGTOGGLEDPDBEADFORCES_H__4D38CB70_23A3_4C6C_8E13_DAB7376E3578__INCLUDED_


#include "LogConstraintMessage.h"

class CLogToggleDPDBeadForces : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleDPDBeadForces(long time, bool bZeroed);

	virtual ~CLogToggleDPDBeadForces();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleDPDBeadForces& rMsg);

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

	CLogToggleDPDBeadForces(const CLogToggleDPDBeadForces& oldMessage);
	CLogToggleDPDBeadForces& operator=(const CLogToggleDPDBeadForces& rhs);


	// ****************************************
	// Data members
private:

	const bool m_bZeroed;	// Flag showing whether DPD forces are zeroed or not

};

#endif // !defined(AFX_LOGTOGGLEDPDBEADFORCES_H__4D38CB70_23A3_4C6C_8E13_DAB7376E3578__INCLUDED_)
