// taCommandLabel.h: interface for the taCommandLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TACOMMANDLABEL_H__C5830C36_3B65_4C9C_9843_DC3AFECAD549__INCLUDED_)
#define AFX_TACOMMANDLABEL_H__C5830C36_3B65_4C9C_9843_DC3AFECAD549__INCLUDED_



#include "taLabelDecorator.h"

class taCommandLabel : public taLabelDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taCommandLabel(const zString label, CCommandTargetNode* const pDec);

	virtual ~taCommandLabel();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:



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

protected:


private:


};

#endif // !defined(AFX_TACOMMANDLABEL_H__C5830C36_3B65_4C9C_9843_DC3AFECAD549__INCLUDED_)
