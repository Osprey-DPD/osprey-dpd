// cgcSetArgumentToRealSequenceImpl.h: interface for the cgcSetArgumentToRealSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOREALSEQUENCEIMPL_H__C9E9751D_F7C2_41E9_926C_FF37AFB0091F__INCLUDED_)
#define AFX_CGCSETARGUMENTTOREALSEQUENCEIMPL_H__C9E9751D_F7C2_41E9_926C_FF37AFB0091F__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToRealSequenceImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToRealSequenceImpl();

	virtual ~cgcSetArgumentToRealSequenceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToRealSequence(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOREALSEQUENCEIMPL_H__C9E9751D_F7C2_41E9_926C_FF37AFB0091F__INCLUDED_)
