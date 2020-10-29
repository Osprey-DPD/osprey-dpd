// LogSetRunCompleteInterval.h: interface for the CLogSetRunCompleteInterval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSETRUNCOMPLETEINTERVAL_H__1CE881CE_2A9B_4F62_ACE6_3DAE82A64596__INCLUDED_)
#define AFX_LOGSETRUNCOMPLETEINTERVAL_H__1CE881CE_2A9B_4F62_ACE6_3DAE82A64596__INCLUDED_


#include "LogInfoMessage.h"

class CLogSetRunCompleteInterval : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogSetRunCompleteInterval(long time, const long interval);

	virtual ~CLogSetRunCompleteInterval();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogSetRunCompleteInterval& rMsg);

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

	CLogSetRunCompleteInterval(const CLogSetRunCompleteInterval& oldMessage);
	CLogSetRunCompleteInterval& operator=(const CLogSetRunCompleteInterval& rhs);


	// ****************************************
	// Data members
private:

	const long m_Interval;	// Percentage of total run length at which to output messages

};

#endif // !defined(AFX_LOGSETRUNCOMPLETEINTERVAL_H__1CE881CE_2A9B_4F62_ACE6_3DAE82A64596__INCLUDED_)
