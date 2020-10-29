// LogToggleRestartWarningMessages.h: interface for the CLogToggleRestartWarningMessages class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGTOGGLERESTARTWARNINGMESSAGES_H__691E7BF4_E371_4E46_B709_4CB6AB4FBB28__INCLUDED_)
#define AFX_LOGTOGGLERESTARTWARNINGMESSAGES_H__691E7BF4_E371_4E46_B709_4CB6AB4FBB28__INCLUDED_


#include "LogInfoMessage.h"

class CLogToggleRestartWarningMessages : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogToggleRestartWarningMessages(long time, bool bShow);

	virtual ~CLogToggleRestartWarningMessages();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogToggleRestartWarningMessages& rMsg);

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

	CLogToggleRestartWarningMessages(const CLogToggleRestartWarningMessages& oldMessage);
	CLogToggleRestartWarningMessages& operator=(const CLogToggleRestartWarningMessages& rhs);


	// ****************************************
	// Data members
private:
	
	const bool m_bShowMessages;	// Flag showing whether messages are logged or not

};

#endif // !defined(AFX_LOGTOGGLERESTARTWARNINGMESSAGES_H__691E7BF4_E371_4E46_B709_4CB6AB4FBB28__INCLUDED_)
