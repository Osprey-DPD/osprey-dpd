// LogacNetworkNotModifiable.h: interface for the CLogacNetworkNotModifiable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACNETWORKNOTMODIFIABLE_H__DEA435E3_FF76_4156_A723_B8078898B34F__INCLUDED_)
#define AFX_LOGACNETWORKNOTMODIFIABLE_H__DEA435E3_FF76_4156_A723_B8078898B34F__INCLUDED_


#include "LogWarningMessage.h"

class CLogacNetworkNotModifiable : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacNetworkNotModifiable(long time, const zString type);

	virtual ~CLogacNetworkNotModifiable();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacNetworkNotModifiable& rMsg);

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

	CLogacNetworkNotModifiable(const CLogacNetworkNotModifiable& oldMessage);
	CLogacNetworkNotModifiable& operator=(const CLogacNetworkNotModifiable& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Type;			// Target network

};

#endif // !defined(AFX_LOGACNETWORKNOTMODIFIABLE_H__DEA435E3_FF76_4156_A723_B8078898B34F__INCLUDED_)
