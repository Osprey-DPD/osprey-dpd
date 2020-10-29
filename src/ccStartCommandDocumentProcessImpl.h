// ccStartCommandDocumentProcessImpl.h: interface for the ccStartCommandDocumentProcessImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCSTARTCOMMANDDOCUMENTPROCESSIMPL_H__17DDC322_9289_4599_99A9_B0F5F94CE21F__INCLUDED_)
#define AFX_CCSTARTCOMMANDDOCUMENTPROCESSIMPL_H__17DDC322_9289_4599_99A9_B0F5F94CE21F__INCLUDED_


#include "ISimBoxCmd.h"

class ccStartCommandDocumentProcessImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ccStartCommandDocumentProcessImpl();

	virtual ~ccStartCommandDocumentProcessImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void StartCommandDocumentProcess(const xxCommand* const pCommand);


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


#endif // !defined(AFX_CCSTARTCOMMANDDOCUMENTPROCESSIMPL_H__17DDC322_9289_4599_99A9_B0F5F94CE21F__INCLUDED_)
