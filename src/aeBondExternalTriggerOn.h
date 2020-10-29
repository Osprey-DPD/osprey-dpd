// aeBondExternalTriggerOn.h: interface for the aeBondExternalTriggerOn class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDEXTERNALTRIGGERON_H__5D397BE3_44FC_474F_8E54_0D72286B623D__INCLUDED_)
#define AFX_AEBONDEXTERNALTRIGGERON_H__5D397BE3_44FC_474F_8E54_0D72286B623D__INCLUDED_


#include "aeBondOnCondition.h"

class aeBondExternalTriggerOn : public aeBondOnCondition  
{
	// ****************************************
	// Construction/Destruction 
public:

	aeBondExternalTriggerOn();

	virtual ~aeBondExternalTriggerOn();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes

	// The first checks if a free active bond can bind to a polymerised bond
	// by attaching to its head; the second checks for addition to the polymerised
	// bond's tail. Note that activation refers to the polymerised bond not the
	// free bond being added.

	virtual bool ActivateHead(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const;
	virtual bool ActivateTail(const aeActiveBond* const pEnd, const aeActiveBond* const pFree) const;
	
	// ****************************************
	// Public access functions
public:


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

#endif // !defined(AFX_AEBONDEXTERNALTRIGGERON_H__5D397BE3_44FC_474F_8E54_0D72286B623D__INCLUDED_)
