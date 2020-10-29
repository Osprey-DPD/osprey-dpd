// LogToggleDPDBeadConservativeForces.h: interface for the CLogToggleDPDBeadConservativeForces class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLEDPDBEADCONSERVATIVEFORCES_H__DF5E571F_490A_4DEC_8D14_B4E905C8F4E9__INCLUDED_)
#define AFX_LOGTOGGLEDPDBEADCONSERVATIVEFORCES_H__DF5E571F_490A_4DEC_8D14_B4E905C8F4E9__INCLUDED_


#include "LogConstraintMessage.h"

class CLogToggleDPDBeadConservativeForces : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleDPDBeadConservativeForces(long time, bool bZeroed);

	virtual ~CLogToggleDPDBeadConservativeForces();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleDPDBeadConservativeForces& rMsg);

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

	CLogToggleDPDBeadConservativeForces(const CLogToggleDPDBeadConservativeForces& oldMessage);
	CLogToggleDPDBeadConservativeForces& operator=(const CLogToggleDPDBeadConservativeForces& rhs);


	// ****************************************
	// Data members
private:

	const bool m_bZeroed;	// Flag showing whether DPD forces are zeroed or not

};
#endif // !defined(AFX_LOGTOGGLEDPDBEADCONSERVATIVEFORCES_H__DF5E571F_490A_4DEC_8D14_B4E905C8F4E9__INCLUDED_)
