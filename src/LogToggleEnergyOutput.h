// LogToggleEnergyOutput.h: interface for the CLogToggleEnergyOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLEENERGYOUTPUT_H__50849EA6_382F_4C4A_97E0_364DDA529512__INCLUDED_)
#define AFX_LOGTOGGLEENERGYOUTPUT_H__50849EA6_382F_4C4A_97E0_364DDA529512__INCLUDED_


#include "LogInfoMessage.h"

class CLogToggleEnergyOutput : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleEnergyOutput(long time, bool bSave);

	virtual ~CLogToggleEnergyOutput();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleEnergyOutput& rMsg);

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

	CLogToggleEnergyOutput(const CLogToggleEnergyOutput& oldMessage);
	CLogToggleEnergyOutput& operator=(const CLogToggleEnergyOutput& rhs);


	// ****************************************
	// Data members
private:

	const bool m_bSave;		// Flag showing whether bead energy is being saved or not
};

#endif // !defined(AFX_LOGTOGGLEENERGYOUTPUT_H__50849EA6_382F_4C4A_97E0_364DDA529512__INCLUDED_)
