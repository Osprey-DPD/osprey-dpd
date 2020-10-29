// LogLamellaPoreClosed.h: interface for the CLogLamellaPoreClosed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLAMELLAPORECLOSED_H__B03F3E0C_36B6_4C05_B9CA_887E2D3493A6__INCLUDED_)
#define AFX_LOGLAMELLAPORECLOSED_H__B03F3E0C_36B6_4C05_B9CA_887E2D3493A6__INCLUDED_


#include "LogEventMessage.h"

class CLogLamellaPoreClosed : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogLamellaPoreClosed(long time, const zString name, long type);

	virtual ~CLogLamellaPoreClosed();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogLamellaPoreClosed& rMsg);

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

	CLogLamellaPoreClosed(const CLogLamellaPoreClosed& oldMessage);
	CLogLamellaPoreClosed& operator=(const CLogLamellaPoreClosed& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name;				// Aggregate name, e.g., bilayer
	const long    m_Type;				// Aggregate id, e.g., 0, 1, 2

};

#endif // !defined(AFX_LOGLAMELLAPORECLOSED_H__B03F3E0C_36B6_4C05_B9CA_887E2D3493A6__INCLUDED_)

