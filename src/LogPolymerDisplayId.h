// LogPolymerDisplayId.h: interface for the CLogPolymerDisplayIdId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGPOLYMERDISPLAYID_H__81A2A837_0BEC_41C0_9458_E7BC8286E297__INCLUDED_)
#define AFX_LOGPOLYMERDISPLAYID_H__81A2A837_0BEC_41C0_9458_E7BC8286E297__INCLUDED_


#include "LogInfoMessage.h" 

class CLogPolymerDisplayId : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogPolymerDisplayId(long time, const zString name, long type, long displayId);

	virtual ~CLogPolymerDisplayId();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogPolymerDisplayId& rMsg);

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

	CLogPolymerDisplayId(const CLogPolymerDisplayId& oldMessage);
	CLogPolymerDisplayId& operator=(const CLogPolymerDisplayId& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name;		// Name of polymer type whose display id is being modified
	const long    m_Type;		// Numeric identifier for polymer type
	const long    m_DisplayId;	// Display id for polymer type
};

#endif // !defined(AFX_LOGPOLYMERDISPLAYID_H__81A2A837_0BEC_41C0_9458_E7BC8286E297__INCLUDED_)
