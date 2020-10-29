// LogBeadDisplayId.h: interface for the CLogBeadDisplayId class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGBEADDISPLAYID_H__0E195341_DDDD_4D6D_A084_02620A4C2E2C__INCLUDED_)
#define AFX_LOGBEADDISPLAYID_H__0E195341_DDDD_4D6D_A084_02620A4C2E2C__INCLUDED_


#include "LogInfoMessage.h" 

class CLogBeadDisplayId : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogBeadDisplayId(long time, const zString name, long type, long displayId);

	virtual ~CLogBeadDisplayId();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogBeadDisplayId& rMsg);

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

	CLogBeadDisplayId(const CLogBeadDisplayId& oldMessage);
	CLogBeadDisplayId& operator=(const CLogBeadDisplayId& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name;		// Name of bead type whose display id is being modified
	const long    m_Type;		// Numeric identifier for bead type
	const long    m_DisplayId;	// Display id for polymer type
};

#endif // !defined(AFX_LOGBEADDISPLAYID_H__0E195341_DDDD_4D6D_A084_02620A4C2E2C__INCLUDED_)
