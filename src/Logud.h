// Logud.h: interface for the CLogud class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGUD_H__FE5B364B_735E_4DFF_89C1_396303AE0641__INCLUDED_)
#define AFX_LOGUD_H__FE5B364B_735E_4DFF_89C1_396303AE0641__INCLUDED_


#include "LogInfoMessage.h"

class CLogud : public CLogInfoMessage   
{
	// ****************************************
	// Construction/Destruction
public:

	CLogud(long time, const zString ud);

	virtual ~CLogud();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogud& rMsg);

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

	CLogud(const CLogud& oldMessage);
	CLogud& operator=(const CLogud& rhs);


	// ****************************************
	// Data members
private:

	const zString m_ud;
};

#endif // !defined(AFX_LOGUD_H__FE5B364B_735E_4DFF_89C1_396303AE0641__INCLUDED_)
