// cgcSetArgumentToIntegerSequenceImpl.h: interface for the cgcSetArgumentToIntegerSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOINTEGERSEQUENCEIMPL_H__F3C5FF0A_447B_4C5C_B04F_F71B50FBBB21__INCLUDED_)
#define AFX_CGCSETARGUMENTTOINTEGERSEQUENCEIMPL_H__F3C5FF0A_447B_4C5C_B04F_F71B50FBBB21__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToIntegerSequenceImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToIntegerSequenceImpl();

	virtual ~cgcSetArgumentToIntegerSequenceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToIntegerSequence(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOINTEGERSEQUENCEIMPL_H__F3C5FF0A_447B_4C5C_B04F_F71B50FBBB21__INCLUDED_)
