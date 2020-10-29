// mcSetAnalysisPeriodImpl.h: interface for the mcSetAnalysisPeriodImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCSETANALYSISPERIODIMPL_H__3945B548_1B02_4D50_B644_E72BF31E3D29__INCLUDED_)
#define AFX_MCSETANALYSISPERIODIMPL_H__3945B548_1B02_4D50_B644_E72BF31E3D29__INCLUDED_


// Forward declarations

class xxCommand;

#include "IMonitorCmd.h"

class mcSetAnalysisPeriodImpl : public virtual IMonitorCmd
{
public:
	// ****************************************
	// Construction/Destruction
public:

	mcSetAnalysisPeriodImpl();

	virtual ~mcSetAnalysisPeriodImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void SetAnalysisPeriod(const xxCommand* const pCommand);


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

#endif // !defined(AFX_MCSETANALYSISPERIODIMPL_H__3945B548_1B02_4D50_B644_E72BF31E3D29__INCLUDED_)
