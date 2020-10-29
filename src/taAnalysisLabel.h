// taAnalysisLabel.h: interface for the taAnalysisLabel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAANALYSISLABEL_H__85F9514A_CC14_4756_A81D_B4BA208CA72E__INCLUDED_)
#define AFX_TAANALYSISLABEL_H__85F9514A_CC14_4756_A81D_B4BA208CA72E__INCLUDED_



#include "taLabelDecorator.h"

class taAnalysisLabel : public taLabelDecorator
{
public:
	// ****************************************
	// Construction/Destruction
public:

	taAnalysisLabel(const zString label, CCommandTargetNode* const pDec);

	virtual ~taAnalysisLabel();


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

#endif // !defined(AFX_TAANALYSISLABEL_H__85F9514A_CC14_4756_A81D_B4BA208CA72E__INCLUDED_)
