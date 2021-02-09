// mcSavePolymerBeadRDFImpl.h: interface for the mcSavePolymerBeadRDFImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSAVEPOLYMERBEADRDFIMPL_H__cb1e8779_9254_40bc_985d_daa48f074f31__INCLUDED_)
#define AFX_MCSAVEPOLYMERBEADRDFIMPL_H__cb1e8779_9254_40bc_985d_daa48f074f31__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSavePolymerBeadRDFImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSavePolymerBeadRDFImpl();

	virtual ~mcSavePolymerBeadRDFImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SavePolymerBeadRDF(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSAVEPOLYMERBEADRDFIMPL_H__cb1e8779_9254_40bc_985d_daa48f074f31__INCLUDED_)
