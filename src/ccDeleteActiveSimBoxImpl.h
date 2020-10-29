// ccDeleteActiveSimBoxImpl.h: interface for the ccDeleteActiveSimBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCDELETEACTIVESIMBOXIMPL_H__4AEFA641_D7D6_4BDF_84DF_412AB374D3E8__INCLUDED_)
#define AFX_CCDELETEACTIVESIMBOXIMPL_H__4AEFA641_D7D6_4BDF_84DF_412AB374D3E8__INCLUDED_


#include "ISimBoxCmd.h"

class ccDeleteActiveSimBoxImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccDeleteActiveSimBoxImpl();

	virtual ~ccDeleteActiveSimBoxImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void DeleteActiveSimBox(const xxCommand* const pCommand);


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
#endif // !defined(AFX_CCDELETEACTIVESIMBOXIMPL_H__4AEFA641_D7D6_4BDF_84DF_412AB374D3E8__INCLUDED_)
