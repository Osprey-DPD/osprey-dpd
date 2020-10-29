// LogBeadDisplay.h: interface for the CLogBeadDisplay class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGBEADDISPLAY_H__3A874CAB_0D75_4247_ADC3_5E9A0CC5AF15__INCLUDED_)
#define AFX_LOGBEADDISPLAY_H__3A874CAB_0D75_4247_ADC3_5E9A0CC5AF15__INCLUDED_


#include "LogInfoMessage.h" 

class CLogBeadDisplay : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogBeadDisplay(long time, const zString name, long type, bool bDisplay);

	virtual ~CLogBeadDisplay();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogBeadDisplay& rMsg);

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

	CLogBeadDisplay(const CLogBeadDisplay& oldMessage);
	CLogBeadDisplay& operator=(const CLogBeadDisplay& rhs);

	// ****************************************
	// Data members
private:

	const zString m_BeadName;	// Name of bead type to hide/display
	const long    m_BeadType;	// Bead type to hide/display
	const bool    m_bDisplay;	// Bead display status
};

#endif // !defined(AFX_LOGBEADDISPLAY_H__3A874CAB_0D75_4247_ADC3_5E9A0CC5AF15__INCLUDED_)
