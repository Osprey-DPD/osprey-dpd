// aeBondOffCondition.h: interface for the aeBondOffCondition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AEBONDOFFCONDITION_H__C1B22D22_9801_4E03_AD73_17F97B944C38__INCLUDED_)
#define AFX_AEBONDOFFCONDITION_H__C1B22D22_9801_4E03_AD73_17F97B944C38__INCLUDED_


// Forward declarations

class aeActiveBond;


class aeBondOffCondition  
{
	// ****************************************
	// Construction/Destruction - protected constructor declared below
public:

	virtual ~aeBondOffCondition();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// The following pure virtual functions must be provided by all derived classes
	// to define the conditions under which active bonds can dissociate.

	// The first checks if an active bond can dissociate from the bond 
	// connected to its head, the second checks if it can dissociate from 
	// that connected to its tail.

	virtual bool ActivateHead(const aeActiveBond* const pEnd) const = 0;
	virtual bool ActivateTail(const aeActiveBond* const pEnd) const = 0;


	// ****************************************
	// Public access functions
public:


	// ****************************************
	// Protected local functions
protected:

	aeBondOffCondition();

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:


	// ****************************************
	// Data members

private:

};

#endif // !defined(AFX_AEBONDOFFCONDITION_H__C1B22D22_9801_4E03_AD73_17F97B944C38__INCLUDED_)
