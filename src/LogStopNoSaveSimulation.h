// LogStopNoSaveSimulation.h: interface for the CLogStopNoSaveSimulation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSTOPNOSAVESIMULATION_H__DB48EF76_6F9A_4E6C_9B56_E5EE3A41C6CE__INCLUDED_)
#define AFX_LOGSTOPNOSAVESIMULATION_H__DB48EF76_6F9A_4E6C_9B56_E5EE3A41C6CE__INCLUDED_


#include "LogInfoMessage.h" 

class CLogStopNoSaveSimulation : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogStopNoSaveSimulation(long time, long stopTime);

	virtual ~CLogStopNoSaveSimulation();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogStopNoSaveSimulation& rMsg);

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

	CLogStopNoSaveSimulation(const CLogStopNoSaveSimulation& oldMessage);
	CLogStopNoSaveSimulation& operator=(const CLogStopNoSaveSimulation& rhs);

	// ****************************************
	// Data members
private:

	const long m_StopTime;	// Time at which simulation is to end

};

#endif // !defined(AFX_LOGSTOPNOSAVESIMULATION_H__DB48EF76_6F9A_4E6C_9B56_E5EE3A41C6CE__INCLUDED_)
