// ctWorkDoneOnBeadTypeInTargetImpl.h: interface for the ctWorkDoneOnBeadTypeInTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTWORKDONEONBEADTYPEINTARGETIMPL_H__20ADAC05_1F06_434D_AA53_B1FA280FEAB3__INCLUDED_)
#define AFX_CTWORKDONEONBEADTYPEINTARGETIMPL_H__20ADAC05_1F06_434D_AA53_B1FA280FEAB3__INCLUDED_


#include "ISimBoxCmd.h"

class ctWorkDoneOnBeadTypeInTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctWorkDoneOnBeadTypeInTargetImpl();

	virtual ~ctWorkDoneOnBeadTypeInTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void WorkDoneOnBeadTypeInTarget(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTWORKDONEONBEADTYPEINTARGETIMPL_H__20ADAC05_1F06_434D_AA53_B1FA280FEAB3__INCLUDED_)
