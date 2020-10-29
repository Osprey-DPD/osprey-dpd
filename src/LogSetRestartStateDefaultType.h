// LogSetRestartStateDefaultType.h: interface for the CLogSetRestartStateDefaultType class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETRESTARTSTATEDEFAULTTYPE_H__531B2FBD_C2A9_4513_B309_0F2EEDAB6D3D__INCLUDED_)
#define AFX_LOGSETRESTARTSTATEDEFAULTTYPE_H__531B2FBD_C2A9_4513_B309_0F2EEDAB6D3D__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetRestartStateDefaultType : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetRestartStateDefaultType(long time, const zString newFormat);

	virtual ~CLogSetRestartStateDefaultType();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetRestartStateDefaultType& rMsg);

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

	CLogSetRestartStateDefaultType(const CLogSetRestartStateDefaultType& oldMessage);
	CLogSetRestartStateDefaultType& operator=(const CLogSetRestartStateDefaultType& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Format; // Restart state format identifying string, e.g., "Bead coordinates" or "Inclusive".
};

#endif // !defined(AFX_LOGSETRESTARTSTATEDEFAULTTYPE_H__531B2FBD_C2A9_4513_B309_0F2EEDAB6D3D__INCLUDED_)
