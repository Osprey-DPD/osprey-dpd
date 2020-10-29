// ctBinEventSuccessIntervalsImpl.h: interface for the ctBinEventSuccessIntervalsImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTBINEVENTSUCCESSINTERVALSIMPL_H__A8638238_4900_4C8C_80AB_CA5C1C23D7AD__INCLUDED_)
#define AFX_CTBINEVENTSUCCESSINTERVALSIMPL_H__A8638238_4900_4C8C_80AB_CA5C1C23D7AD__INCLUDED_


#include "ISimBoxCmd.h"

class ctBinEventSuccessIntervalsImpl : public virtual ISimBoxCmd
{
	// ****************************************
	// Construction/Destruction
public:

	ctBinEventSuccessIntervalsImpl();

	virtual ~ctBinEventSuccessIntervalsImpl();
	
	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	// ****************************************
	// Public access functions
public:

	void BinEventSuccessIntervals(const xxCommand* const pCommand);


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

#endif // !defined(AFX_CTBINEVENTSUCCESSINTERVALSIMPL_H__A8638238_4900_4C8C_80AB_CA5C1C23D7AD__INCLUDED_)
