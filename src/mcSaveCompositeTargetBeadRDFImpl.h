// mcSaveCompositeTargetBeadRDFImpl.h: interface for the mcSaveCompositeTargetBeadRDFImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSaveCompositeTargetBeadRDFIMPL_H__efa55814_382c_4b23_aa95_140726858af5__INCLUDED_)
#define AFX_MCSaveCompositeTargetBeadRDFIMPL_H__efa55814_382c_4b23_aa95_140726858af5__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSaveCompositeTargetBeadRDFImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSaveCompositeTargetBeadRDFImpl();

	virtual ~mcSaveCompositeTargetBeadRDFImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SaveCompositeTargetBeadRDF(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSaveCompositeTargetBeadRDFIMPL_H__efa55814_382c_4b23_aa95_140726858af5__INCLUDED_)
