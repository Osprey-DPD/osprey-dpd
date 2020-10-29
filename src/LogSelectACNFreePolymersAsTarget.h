// LogSelectACNFreePolymersAsTarget.h: interface for the CLogSelectACNFreePolymersAsTarget class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSELECTACNFREEPOLYMERSASTARGET_H__80DE2822_4154_404A_A45C_B647A3AAE261__INCLUDED_)
#define AFX_LOGSELECTACNFREEPOLYMERSASTARGET_H__80DE2822_4154_404A_A45C_B647A3AAE261__INCLUDED_


#include "LogConstraintMessage.h"

class CLogSelectACNFreePolymersAsTarget : public CLogConstraintMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSelectACNFreePolymersAsTarget(long time, const zString cmdTarget, const zString acnType);

	virtual ~CLogSelectACNFreePolymersAsTarget();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSelectACNFreePolymersAsTarget& rMsg);

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

	CLogSelectACNFreePolymersAsTarget(const CLogSelectACNFreePolymersAsTarget& oldMessage);
	CLogSelectACNFreePolymersAsTarget& operator=(const CLogSelectACNFreePolymersAsTarget& rhs);


	// ****************************************
	// Data members
private:

	const zString m_CmdTarget;			// Command target name
	const zString m_ACNType;			// ACN name
};

#endif // !defined(AFX_LOGSELECTACNFREEPOLYMERSASTARGET_H__80DE2822_4154_404A_A45C_B647A3AAE261__INCLUDED_)
