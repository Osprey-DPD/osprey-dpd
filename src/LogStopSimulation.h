// LogStopSimulation.h: interface for the CLogStopSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSTOPSIMULATION_H__64B0753E_B93E_428B_8160_B64858CC0AB2__INCLUDED_)
#define AFX_LOGSTOPSIMULATION_H__64B0753E_B93E_428B_8160_B64858CC0AB2__INCLUDED_


#include "LogInfoMessage.h" 

class CLogStopSimulation : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogStopSimulation(long time, long stopTime);

	virtual ~CLogStopSimulation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogStopSimulation& rMsg);

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

	CLogStopSimulation(const CLogStopSimulation& oldMessage);
	CLogStopSimulation& operator=(const CLogStopSimulation& rhs);

	// ****************************************
	// Data members
private:

	const long m_StopTime;	// Time at which simulation is to end

};

#endif // !defined(AFX_LOGSTOPSIMULATION_H__64B0753E_B93E_428B_8160_B64858CC0AB2__INCLUDED_)
