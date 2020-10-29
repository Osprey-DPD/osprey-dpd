// LogacSetFreeActiveBondsVisible.h: interface for the CLogacSetFreeActiveBondsVisible class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFREEACTIVEBONDSVISIBLE_H__A757D0CB_83FF_4693_8A8F_89B98152B1BD__INCLUDED_)
#define AFX_LOGACSETFREEACTIVEBONDSVISIBLE_H__A757D0CB_83FF_4693_8A8F_89B98152B1BD__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSetFreeActiveBondsVisible : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFreeActiveBondsVisible(long time, const zString type);

	virtual ~CLogacSetFreeActiveBondsVisible();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFreeActiveBondsVisible& rMsg);

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

	CLogacSetFreeActiveBondsVisible(const CLogacSetFreeActiveBondsVisible& oldMessage);
	CLogacSetFreeActiveBondsVisible& operator=(const CLogacSetFreeActiveBondsVisible& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
};

#endif // !defined(AFX_LOGACSETFREEACTIVEBONDSVISIBLE_H__A757D0CB_83FF_4693_8A8F_89B98152B1BD__INCLUDED_)
