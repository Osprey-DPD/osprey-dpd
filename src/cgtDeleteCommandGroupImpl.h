// cgtDeleteCommandGroupImpl.h: interface for the cgtDeleteCommandGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTDELETECOMMANDGROUPIMPL_H__D9CFE161_B964_4D85_9BD7_F205A1F61F3B__INCLUDED_)
#define AFX_CGTDELETECOMMANDGROUPIMPL_H__D9CFE161_B964_4D85_9BD7_F205A1F61F3B__INCLUDED_



#include "ISimBoxCmd.h"

class cgtDeleteCommandGroupImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtDeleteCommandGroupImpl();

	virtual ~cgtDeleteCommandGroupImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void InternalDeleteCommandGroup(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGTDELETECOMMANDGROUPIMPL_H__D9CFE161_B964_4D85_9BD7_F205A1F61F3B__INCLUDED_)
