// ccToggleDPDBeadConservativeForcesImpl.h: interface for the ccToggleDPDBeadConservativeForcesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCTOGGLEDPDBEADCONSERVATIVEFORCESIMPL_H__406E8333_14DA_4086_A230_52136D96DB8D__INCLUDED_)
#define AFX_CCTOGGLEDPDBEADCONSERVATIVEFORCESIMPL_H__406E8333_14DA_4086_A230_52136D96DB8D__INCLUDED_


#include "ISimBoxCmd.h"

class ccToggleDPDBeadConservativeForcesImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccToggleDPDBeadConservativeForcesImpl();

	virtual ~ccToggleDPDBeadConservativeForcesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleDPDBeadConservativeForces(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCTOGGLEDPDBEADCONSERVATIVEFORCESIMPL_H__406E8333_14DA_4086_A230_52136D96DB8D__INCLUDED_)
