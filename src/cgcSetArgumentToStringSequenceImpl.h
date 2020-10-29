// cgcSetArgumentToStringSequenceImpl.h: interface for the cgcSetArgumentToStringSequenceImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGCSETARGUMENTTOSTRINGSEQUENCEIMPL_H__70EF5390_BC61_4583_920D_8D55F1250384__INCLUDED_)
#define AFX_CGCSETARGUMENTTOSTRINGSEQUENCEIMPL_H__70EF5390_BC61_4583_920D_8D55F1250384__INCLUDED_



#include "ISimBoxCmd.h"

class cgcSetArgumentToStringSequenceImpl : public virtual ISimBoxCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	cgcSetArgumentToStringSequenceImpl();

	virtual ~cgcSetArgumentToStringSequenceImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetArgumentToStringSequence(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CGCSETARGUMENTTOSTRINGSEQUENCEIMPL_H__70EF5390_BC61_4583_920D_8D55F1250384__INCLUDED_)
