// LogSetTimeStepSize.h: interface for the CLogSetTimeStepSize class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETTIMESTEPSIZE_H__3E76B6FB_70BD_457D_BB3B_C67CFCD9FAA5__INCLUDED_)
#define AFX_LOGSETTIMESTEPSIZE_H__3E76B6FB_70BD_457D_BB3B_C67CFCD9FAA5__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSetTimeStepSize : public CLogConstraintMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetTimeStepSize(long time, double dt);

	virtual ~CLogSetTimeStepSize();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetTimeStepSize& rMsg);

	// ****************************************
	// Public access functions
public:

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const;

	// ****************************************
	// Protected local functions
protected:

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:
	
	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	CLogSetTimeStepSize(const CLogSetTimeStepSize& oldMessage);
	CLogSetTimeStepSize& operator=(const CLogSetTimeStepSize& rhs);


	// ****************************************
	// Data members
private:

	const double m_dt;	// Integration time step
};


#endif // !defined(AFX_LOGSETTIMESTEPSIZE_H__3E76B6FB_70BD_457D_BB3B_C67CFCD9FAA5__INCLUDED_)
