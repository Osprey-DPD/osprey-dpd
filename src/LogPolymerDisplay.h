// LogPolymerDisplay.h: interface for the CLogPolymerDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPOLYMERDISPLAY_H__E499CED6_1BAC_46A8_84A2_05DD82ECD191__INCLUDED_)
#define AFX_LOGPOLYMERDISPLAY_H__E499CED6_1BAC_46A8_84A2_05DD82ECD191__INCLUDED_


#include "LogInfoMessage.h" 

class CLogPolymerDisplay : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogPolymerDisplay(long time, const zString name, long type, bool bDisplay, bool bModified);

	virtual ~CLogPolymerDisplay();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogPolymerDisplay& rMsg);

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

	CLogPolymerDisplay(const CLogPolymerDisplay& oldMessage);
	CLogPolymerDisplay& operator=(const CLogPolymerDisplay& rhs);

	// ****************************************
	// Data members
private:

	const zString m_PolymerName;	// Name of polymer type to hide/display
	const long    m_PolymerType;	// Polymer type to hide/display
	const bool    m_bDisplay;		// Polymer display status
	const bool    m_bModified;		// Flag showing if the polymer's display status was changed by command
};

#endif // !defined(AFX_LOGPOLYMERDISPLAY_H__E499CED6_1BAC_46A8_84A2_05DD82ECD191__INCLUDED_)
