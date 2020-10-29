// LogToggleSliceEnergyOutput.h: interface for the CLogToggleSliceEnergyOutput class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLESLICEENERGYOUTPUT_H__AEC0671E_685C_409C_997F_AAA9D8A62589__INCLUDED_)
#define AFX_LOGTOGGLESLICEENERGYOUTPUT_H__AEC0671E_685C_409C_997F_AAA9D8A62589__INCLUDED_


#include "LogInfoMessage.h"

class CLogToggleSliceEnergyOutput : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleSliceEnergyOutput(long time, bool bSave);

	virtual ~CLogToggleSliceEnergyOutput();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleSliceEnergyOutput& rMsg);

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

	CLogToggleSliceEnergyOutput(const CLogToggleSliceEnergyOutput& oldMessage);
	CLogToggleSliceEnergyOutput& operator=(const CLogToggleSliceEnergyOutput& rhs);


	// ****************************************
	// Data members
private:

	const bool m_bSave;		// Flag showing whether the enrgy of beads in a slice is being saved or not
};

#endif // !defined(AFX_LOGTOGGLESLICEENERGYOUTPUT_H__AEC0671E_685C_409C_997F_AAA9D8A62589__INCLUDED_)
