// LogSaveProtocolFile.h: interface for the CLogSaveProtocolFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSAVEPROTOCOLFILE_H__E0B28B64_3D01_4518_AAF6_8E0BBC27D2B4__INCLUDED_)
#define AFX_LOGSAVEPROTOCOLFILE_H__E0B28B64_3D01_4518_AAF6_8E0BBC27D2B4__INCLUDED_


#include "LogInfoMessage.h"

class CLogSaveProtocolFile : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSaveProtocolFile(long time, const zString protocol, const zString runId);

	virtual ~CLogSaveProtocolFile();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSaveProtocolFile& rMsg);

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

	CLogSaveProtocolFile(const CLogSaveProtocolFile& oldMessage);
	CLogSaveProtocolFile& operator=(const CLogSaveProtocolFile& rhs);


	// ****************************************
	// Data members
private:

	const zString m_ProtocolId;
	const zString m_RunId;

};

#endif // !defined(AFX_LOGSAVEPROTOCOLFILE_H__E0B28B64_3D01_4518_AAF6_8E0BBC27D2B4__INCLUDED_)
