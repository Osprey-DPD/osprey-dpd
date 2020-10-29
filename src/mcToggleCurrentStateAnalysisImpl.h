// mcToggleCurrentStateAnalysisImpl.h: interface for the mcToggleCurrentStateAnalysisImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCTOGGLECURRENTSTATEANALYSISIMPL_H__E2574BDE_4DD6_406A_B1FE_9E4B438D5C33__INCLUDED_)
#define AFX_MCTOGGLECURRENTSTATEANALYSISIMPL_H__E2574BDE_4DD6_406A_B1FE_9E4B438D5C33__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"


class mcToggleCurrentStateAnalysisImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcToggleCurrentStateAnalysisImpl();

	virtual ~mcToggleCurrentStateAnalysisImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void ToggleCurrentStateAnalysis(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCTOGGLECURRENTSTATEANALYSISIMPL_H__E2574BDE_4DD6_406A_B1FE_9E4B438D5C33__INCLUDED_)
