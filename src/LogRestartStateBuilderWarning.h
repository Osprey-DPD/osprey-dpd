// LogRestartStateBuilderWarning.h: interface for the CLogRestartStateBuilderWarning class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGRESTARTSTATEBUILDERWARNING_H__ED2D718B_262C_4BA0_B03A_E5988B368E42__INCLUDED_)
#define AFX_LOGRESTARTSTATEBUILDERWARNING_H__ED2D718B_262C_4BA0_B03A_E5988B368E42__INCLUDED_


#include "LogWarningMessage.h"

class CLogRestartStateBuilderWarning : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogRestartStateBuilderWarning(long time, const zString message);

	virtual ~CLogRestartStateBuilderWarning();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogRestartStateBuilderWarning& rMsg);

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
	// by declaring them privately but providing NO definitions.

	CLogRestartStateBuilderWarning(const CLogRestartStateBuilderWarning& oldMessage);
	CLogRestartStateBuilderWarning& operator=(const CLogRestartStateBuilderWarning& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Message;	// Message describing the error condition

};

#endif // !defined(AFX_LOGRESTARTSTATEBUILDERWARNING_H__ED2D718B_262C_4BA0_B03A_E5988B368E42__INCLUDED_)
