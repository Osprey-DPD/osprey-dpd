// LogacACNCreateEventSource.h: interface for the CLogacACNCreateEventSource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACACNCREATEEVENTSOURCE_H__26B59210_D00A_4498_AA24_7F013D30F8C2__INCLUDED_)
#define AFX_LOGACACNCREATEEVENTSOURCE_H__26B59210_D00A_4498_AA24_7F013D30F8C2__INCLUDED_


#include "LogInfoMessage.h"

class CLogacACNCreateEventSource : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacACNCreateEventSource(long time, const zString acnType, const zString eventType, const zString sourceId);

	virtual ~CLogacACNCreateEventSource();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacACNCreateEventSource& rMsg);

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

	CLogacACNCreateEventSource(const CLogacACNCreateEventSource& oldMessage);
	CLogacACNCreateEventSource& operator=(const CLogacACNCreateEventSource& rhs);


	// ****************************************
	// Data members
private:

    const zString m_ACNType;     // Name of ACN whose events are being monitored
    const zString m_EventType;   // Name of event type being monitored
    const zString m_SourceId;    // Name of the event source 

};

#endif // !defined(AFX_LOGACACNCREATEEVENTSOURCE_H__26B59210_D00A_4498_AA24_7F013D30F8C2__INCLUDED_)
