// cgtCreateCommandGroupImpl.h: interface for the cgtCreateCommandGroupImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGTCREATECOMMANDGROUPIMPL_H__D253AD63_97B5_4BB7_BAEB_3316231D1CFC__INCLUDED_)
#define AFX_CGTCREATECOMMANDGROUPIMPL_H__D253AD63_97B5_4BB7_BAEB_3316231D1CFC__INCLUDED_



#include "ISimBoxCmd.h"

class cgtCreateCommandGroupImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgtCreateCommandGroupImpl();

	virtual ~cgtCreateCommandGroupImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void InternalCreateCommandGroup(const xxCommand* const pCommand);


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


#endif // !defined(AFX_CGTCREATECOMMANDGROUPIMPL_H__D253AD63_97B5_4BB7_BAEB_3316231D1CFC__INCLUDED_)
