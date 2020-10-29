// ccSelectACNFreeBondsAsTargetImpl.h: interface for the ccSelectACNFreeBondsAsTargetImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSELECTACNFREEBONDSASTARGETIMPL_H__8179360C_4C4B_4981_B380_14E4FEC9BBAC__INCLUDED_)
#define AFX_CCSELECTACNFREEBONDSASTARGETIMPL_H__8179360C_4C4B_4981_B380_14E4FEC9BBAC__INCLUDED_


#include "ISimBoxCmd.h"

class ccSelectACNFreeBondsAsTargetImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccSelectACNFreeBondsAsTargetImpl();

	virtual ~ccSelectACNFreeBondsAsTargetImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SelectACNFreeBondsAsTarget(const xxCommand* const pCommand);

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

#endif // !defined(AFX_CCSELECTACNFREEBONDSASTARGETIMPL_H__8179360C_4C4B_4981_B380_14E4FEC9BBAC__INCLUDED_)
