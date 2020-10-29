// ccStopImpl.h: interface for the ccStopImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSTOPIMPL_H__0148EEBA_2DC6_4C68_8AD2_726B982B3F5B__INCLUDED_)
#define AFX_CCSTOPIMPL_H__0148EEBA_2DC6_4C68_8AD2_726B982B3F5B__INCLUDED_


#include "ISimBoxCmd.h"

class ccStopImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccStopImpl();

	virtual ~ccStopImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void Stop(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCSTOPIMPL_H__0148EEBA_2DC6_4C68_8AD2_726B982B3F5B__INCLUDED_)
