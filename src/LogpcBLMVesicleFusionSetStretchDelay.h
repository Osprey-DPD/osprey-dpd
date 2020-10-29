// LogpcBLMVesicleFusionSetStretchDelay.h: interface for the CLogpcBLMVesicleFusionSetStretchDelay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCBLMVESICLEFUSIONSETSTRETCHDELAY_H__653FFFFA_B991_4B65_B938_560E62A46D27__INCLUDED_)
#define AFX_LOGPCBLMVESICLEFUSIONSETSTRETCHDELAY_H__653FFFFA_B991_4B65_B938_560E62A46D27__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcBLMVesicleFusionSetStretchDelay : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcBLMVesicleFusionSetStretchDelay(long time, const zString pid, long delay);

	virtual ~CLogpcBLMVesicleFusionSetStretchDelay();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcBLMVesicleFusionSetStretchDelay& rMsg);

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

	CLogpcBLMVesicleFusionSetStretchDelay(const CLogpcBLMVesicleFusionSetStretchDelay& oldMessage);
	CLogpcBLMVesicleFusionSetStretchDelay& operator=(const CLogpcBLMVesicleFusionSetStretchDelay& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;	// Target process' id
	const long    m_Delay;	// Delay before applying stretching force
};

#endif // !defined(AFX_LOGPCBLMVESICLEFUSIONSETSTRETCHDELAY_H__653FFFFA_B991_4B65_B938_560E62A46D27__INCLUDED_)
