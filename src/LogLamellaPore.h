// LogLamellaPore.h: interface for the CLogLamellaPore class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGLAMELLAPORE_H__51B49CA3_8D08_47E5_B8F9_15A296B0431F__INCLUDED_)
#define AFX_LOGLAMELLAPORE_H__51B49CA3_8D08_47E5_B8F9_15A296B0431F__INCLUDED_


#include "LogEventMessage.h"

class CLogLamellaPore : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogLamellaPore(long time, const zString name, long type);

	virtual ~CLogLamellaPore();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogLamellaPore& rMsg);

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

	CLogLamellaPore(const CLogLamellaPore& oldMessage);
	CLogLamellaPore& operator=(const CLogLamellaPore& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name;				// Aggregate name, e.g., bilayer
	const long    m_Type;				// Aggregate id, e.g., 0, 1, 2

};

#endif // !defined(AFX_LOGLAMELLAPORE_H__51B49CA3_8D08_47E5_B8F9_15A296B0431F__INCLUDED_)
