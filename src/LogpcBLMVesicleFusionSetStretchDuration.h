// LogpcBLMVesicleFusionSetStretchDuration.h: interface for the CLogpcBLMVesicleFusionSetStretchDuration class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPCBLMVESICLEFUSIONSETSTRETCHDURATION_H__FE77173E_CCDC_4A99_B5BC_0B672CE5A3DA__INCLUDED_)
#define AFX_LOGPCBLMVESICLEFUSIONSETSTRETCHDURATION_H__FE77173E_CCDC_4A99_B5BC_0B672CE5A3DA__INCLUDED_


#include "LogInfoMessage.h"

class CLogpcBLMVesicleFusionSetStretchDuration : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogpcBLMVesicleFusionSetStretchDuration(long time, const zString pid, long duration);

	virtual ~CLogpcBLMVesicleFusionSetStretchDuration();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogpcBLMVesicleFusionSetStretchDuration& rMsg);

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

	CLogpcBLMVesicleFusionSetStretchDuration(const CLogpcBLMVesicleFusionSetStretchDuration& oldMessage);
	CLogpcBLMVesicleFusionSetStretchDuration& operator=(const CLogpcBLMVesicleFusionSetStretchDuration& rhs);


	// ****************************************
	// Data members
private:

	const zString m_pid;		// Target process' id
	const long    m_Duration;	// Duration of stretching force
};

#endif // !defined(AFX_LOGPCBLMVESICLEFUSIONSETSTRETCHDURATION_H__FE77173E_CCDC_4A99_B5BC_0B672CE5A3DA__INCLUDED_)
