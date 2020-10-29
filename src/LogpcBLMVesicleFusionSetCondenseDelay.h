// LogpcBLMVesicleFusionSetCondenseDelay.h: interface for the CLogpcBLMVesicleFusionSetCondenseDelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCBLMVESICLEFUSIONSETCONDENSEDELAY_H__C7D95EA9_4736_4A03_9089_0BB6A69A8318__INCLUDED_)
#define AFX_LOGPCBLMVESICLEFUSIONSETCONDENSEDELAY_H__C7D95EA9_4736_4A03_9089_0BB6A69A8318__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcBLMVesicleFusionSetCondenseDelay : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcBLMVesicleFusionSetCondenseDelay(long time, const zString pid, long delay);

	virtual ~CLogpcBLMVesicleFusionSetCondenseDelay();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcBLMVesicleFusionSetCondenseDelay& rMsg);

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

	CLogpcBLMVesicleFusionSetCondenseDelay(const CLogpcBLMVesicleFusionSetCondenseDelay& oldMessage);
	CLogpcBLMVesicleFusionSetCondenseDelay& operator=(const CLogpcBLMVesicleFusionSetCondenseDelay& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;	// Target process' id
	const long    m_Delay;	// Delay before applying bending moment
};

#endif // !defined(AFX_LOGPCBLMVESICLEFUSIONSETCONDENSEDELAY_H__C7D95EA9_4736_4A03_9089_0BB6A69A8318__INCLUDED_)
