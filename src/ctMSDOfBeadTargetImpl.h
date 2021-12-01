// ctMSDOfBeadTargetImpl.h: interface for the ctMSDOfBeadTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTMSDOFBEADTARGETIMPL_H__4d271339_b4d3_469e_83f5_144ecc09608c__INCLUDED_)
#define AFX_CTMSDOFBEADTARGETIMPL_H__4d271339_b4d3_469e_83f5_144ecc09608c__INCLUDED_


#include "ISimBoxCmd.h"

class ctMSDOfBeadTargetImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	ctMSDOfBeadTargetImpl();

	virtual ~ctMSDOfBeadTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void MSDBeadTarget(const xxCommand* const pCommand);


	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_CTMSDOFBEADTARGETIMPL_H__4d271339_b4d3_469e_83f5_144ecc09608c__INCLUDED_)
