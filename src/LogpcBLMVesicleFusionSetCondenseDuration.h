// LogpcBLMVesicleFusionSetCondenseDuration.h: interface for the CLogpcBLMVesicleFusionSetCondenseDuration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCBLMVESICLEFUSIONSETCONDENSEDURATION_H__59557C35_4E9B_4D64_8563_DF21E9F51461__INCLUDED_)
#define AFX_LOGPCBLMVESICLEFUSIONSETCONDENSEDURATION_H__59557C35_4E9B_4D64_8563_DF21E9F51461__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcBLMVesicleFusionSetCondenseDuration : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcBLMVesicleFusionSetCondenseDuration(long time, const zString pid, long duration);

	virtual ~CLogpcBLMVesicleFusionSetCondenseDuration();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcBLMVesicleFusionSetCondenseDuration& rMsg);

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

	CLogpcBLMVesicleFusionSetCondenseDuration(const CLogpcBLMVesicleFusionSetCondenseDuration& oldMessage);
	CLogpcBLMVesicleFusionSetCondenseDuration& operator=(const CLogpcBLMVesicleFusionSetCondenseDuration& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;		// Target process' id
	const long    m_Duration;	// Duration of bending moment
};

#endif // !defined(AFX_LOGPCBLMVESICLEFUSIONSETCONDENSEDURATION_H__59557C35_4E9B_4D64_8563_DF21E9F51461__INCLUDED_)
