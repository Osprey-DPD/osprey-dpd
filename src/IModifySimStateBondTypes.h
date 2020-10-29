// IModifySimStateBondTypes.h: interface for the IModifySimStateBondTypes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMODIFYSIMSTATEBONDTYPES_H__35DFAFF4_4AB3_49D5_849D_6FCA771B949F__INCLUDED_)
#define AFX_IMODIFYSIMSTATEBONDTYPES_H__35DFAFF4_4AB3_49D5_849D_6FCA771B949F__INCLUDED_


#include "IModifySimState.h"

class IModifySimStateBondTypes : public IModifySimState  
{
	// ****************************************
	// Construction/Destruction: 
public:

	IModifySimStateBondTypes(CSimState& simState);

	virtual ~IModifySimStateBondTypes();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes 
public:


	// ****************************************
	// Public access functions
public:

	void AddNewType(CBond* const pBond);
	bool AddNewNamedType(CBond* const pBond, long newType, const zString name);
	void AddPolymerisedBond(CBond* const pBond);

	void CalculateTypeTotals();


	// ****************************************
	// Protected local functions
protected:

	

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	IModifySimStateBondTypes(const IModifySimStateBondTypes& oldIModify);
	IModifySimStateBondTypes& operator=(const IModifySimStateBondTypes& rhs);

	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_IMODIFYSIMSTATEBONDTYPES_H__35DFAFF4_4AB3_49D5_849D_6FCA771B949F__INCLUDED_)
