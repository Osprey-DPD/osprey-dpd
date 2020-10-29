// taActionLabel.h: interface for the taActionLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAACTIONLABEL_H__FB9AAA49_D8F2_40B9_9CB8_5308C4AA786C__INCLUDED_)
#define AFX_TAACTIONLABEL_H__FB9AAA49_D8F2_40B9_9CB8_5308C4AA786C__INCLUDED_



#include "taLabelDecorator.h"

class taActionLabel : public taLabelDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taActionLabel(const zString label, CCommandTargetNode* const pDec);

	virtual ~taActionLabel();


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

#endif // !defined(AFX_TAACTIONLABEL_H__FB9AAA49_D8F2_40B9_9CB8_5308C4AA786C__INCLUDED_)
