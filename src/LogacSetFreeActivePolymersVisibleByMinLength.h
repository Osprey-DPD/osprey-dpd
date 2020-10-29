// LogacSetFreeActivePolymersVisibleByMinLength.h: interface for the CLogacSetFreeActivePolymersVisibleByMinLengthByMinLength class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACSETFREEACTIVEPOLYMERSVISIBLEBYMINLENGTH_H__8918A95A_9618_4C11_86E4_FFBC3F7FA985__INCLUDED_)
#define AFX_LOGACSETFREEACTIVEPOLYMERSVISIBLEBYMINLENGTH_H__8918A95A_9618_4C11_86E4_FFBC3F7FA985__INCLUDED_


#include "LogInfoMessage.h"

class CLogacSetFreeActivePolymersVisibleByMinLength : public CLogInfoMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacSetFreeActivePolymersVisibleByMinLength(long time, const zString type, long minSize);

	virtual ~CLogacSetFreeActivePolymersVisibleByMinLength();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacSetFreeActivePolymersVisibleByMinLength& rMsg);

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

	CLogacSetFreeActivePolymersVisibleByMinLength(const CLogacSetFreeActivePolymersVisibleByMinLength& oldMessage);
	CLogacSetFreeActivePolymersVisibleByMinLength& operator=(const CLogacSetFreeActivePolymersVisibleByMinLength& rhs);


	// ****************************************
	// Data members
private:

	const zString	m_ACNType;		// Type of active cell network modified
	const long      m_MinSize;		// Minimum number of monomers in visible polymers
};

#endif // !defined(AFX_LOGACSETFREEACTIVEPOLYMERSVISIBLEBYMINLENGTH_H__8918A95A_9618_4C11_86E4_FFBC3F7FA985__INCLUDED_)
