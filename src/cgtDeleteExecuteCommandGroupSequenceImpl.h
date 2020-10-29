// cgtDeleteExecuteCommandGroupSequenceImpl.h: interface for the cgtDeleteExecuteCommandGroupSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTDELETEEXECUTECOMMANDGROUPSEQUENCEIMPL_H__977A84A8_D313_4ECD_B1A8_C1137EFCC185__INCLUDED_)
#define AFX_CGTDELETEEXECUTECOMMANDGROUPSEQUENCEIMPL_H__977A84A8_D313_4ECD_B1A8_C1137EFCC185__INCLUDED_



#include "ISimBoxCmd.h"

class cgtDeleteExecuteCommandGroupSequenceImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtDeleteExecuteCommandGroupSequenceImpl();

	virtual ~cgtDeleteExecuteCommandGroupSequenceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void DeleteExecuteCommandGroupSequence(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTDELETEEXECUTECOMMANDGROUPSEQUENCEIMPL_H__977A84A8_D313_4ECD_B1A8_C1137EFCC185__INCLUDED_)
