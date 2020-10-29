// aeBondExternalTriggerOff.h: interface for the aeBondExternalTriggerOff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDEXTERNALTRIGGEROFF_H__6703D0BD_4BE9_4865_9BFB_88134602F9A0__INCLUDED_)
#define AFX_AEBONDEXTERNALTRIGGEROFF_H__6703D0BD_4BE9_4865_9BFB_88134602F9A0__INCLUDED_


#include "aeBondOffCondition.h"

class aeBondExternalTriggerOff : public aeBondOffCondition  
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~aeBondExternalTriggerOff();


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

	virtual bool ActivateHead(const aeActiveBond* const pEnd) const = 0;
	virtual bool ActivateTail(const aeActiveBond* const pEnd) const = 0;
	
	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	aeBondExternalTriggerOff();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:

};

#endif // !defined(AFX_AEBONDEXTERNALTRIGGEROFF_H__6703D0BD_4BE9_4865_9BFB_88134602F9A0__INCLUDED_)
