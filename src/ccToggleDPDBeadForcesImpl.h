// ccToggleDPDBeadForcesImpl.h: interface for the ccToggleDPDBeadForcesImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCTOGGLEDPDBEADFORCESIMPL_H__0BB1BF37_427C_46D8_AAB4_3080580012CF__INCLUDED_)
#define AFX_CCTOGGLEDPDBEADFORCESIMPL_H__0BB1BF37_427C_46D8_AAB4_3080580012CF__INCLUDED_


#include "ISimBoxCmd.h"

class ccToggleDPDBeadForcesImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccToggleDPDBeadForcesImpl();

	virtual ~ccToggleDPDBeadForcesImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleDPDBeadForces(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CCTOGGLEDPDBEADFORCESIMPL_H__0BB1BF37_427C_46D8_AAB4_3080580012CF__INCLUDED_)
