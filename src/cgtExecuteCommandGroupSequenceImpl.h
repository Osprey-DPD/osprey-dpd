// cgtExecuteCommandGroupSequenceImpl.h: interface for the cgtExecuteCommandGroupSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTEXECUTECOMMANDGROUPSEQUENCEIMPL_H__5DCCC1E3_70D7_43DE_99E7_441341D1D794__INCLUDED_)
#define AFX_CGTEXECUTECOMMANDGROUPSEQUENCEIMPL_H__5DCCC1E3_70D7_43DE_99E7_441341D1D794__INCLUDED_



#include "ISimBoxCmd.h"

class cgtExecuteCommandGroupSequenceImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtExecuteCommandGroupSequenceImpl();

	virtual ~cgtExecuteCommandGroupSequenceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ExecuteCommandGroupSequence(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTEXECUTECOMMANDGROUPSEQUENCEIMPL_H__5DCCC1E3_70D7_43DE_99E7_441341D1D794__INCLUDED_)
