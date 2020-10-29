// LogacNetworkNotAccessible.h: interface for the CLogacNetworkNotAccessible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACNETWORKNOTACCESSIBLE_H__43B3913A_AABC_4794_BB79_FA56581BBAA9__INCLUDED_)
#define AFX_LOGACNETWORKNOTACCESSIBLE_H__43B3913A_AABC_4794_BB79_FA56581BBAA9__INCLUDED_


#include "LogWarningMessage.h"

class CLogacNetworkNotAccessible : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacNetworkNotAccessible(long time, const zString type, const zString command);

	virtual ~CLogacNetworkNotAccessible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacNetworkNotAccessible& rMsg);

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

	CLogacNetworkNotAccessible(const CLogacNetworkNotAccessible& oldMessage);
	CLogacNetworkNotAccessible& operator=(const CLogacNetworkNotAccessible& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Type;			// Target Active Cell Network
	const zString m_CommandName;	// Command name

};

#endif // !defined(AFX_LOGACNETWORKNOTACCESSIBLE_H__43B3913A_AABC_4794_BB79_FA56581BBAA9__INCLUDED_)
