// LogSaveNonBeadInclusiveRestartState.h: interface for the CLogSaveNonBeadInclusiveRestartState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVENONBEADINCLUSIVERESTARTSTATE_H__68FC24E7_A561_4FBD_AC26_2DFD21C4D7A7__INCLUDED_)
#define AFX_LOGSAVENONBEADINCLUSIVERESTARTSTATE_H__68FC24E7_A561_4FBD_AC26_2DFD21C4D7A7__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveNonBeadInclusiveRestartState : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveNonBeadInclusiveRestartState(long time);

	virtual ~CLogSaveNonBeadInclusiveRestartState();		// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveNonBeadInclusiveRestartState& rMsg);

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

	CLogSaveNonBeadInclusiveRestartState(const CLogSaveNonBeadInclusiveRestartState& oldMessage);
	CLogSaveNonBeadInclusiveRestartState& operator=(const CLogSaveNonBeadInclusiveRestartState& rhs);


	// ****************************************
	// Data members
private:

};

#endif // !defined(AFX_LOGSAVENONBEADINCLUSIVERESTARTSTATE_H__68FC24E7_A561_4FBD_AC26_2DFD21C4D7A7__INCLUDED_)
