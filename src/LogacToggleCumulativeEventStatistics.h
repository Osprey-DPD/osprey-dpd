// LogacToggleCumulativeEventStatistics.h: interface for the CLogacToggleCumulativeEventStatistics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGACTOGGLECUMULATIVEEVENTSTATISTICS_H__3F6DEBA5_BF46_4697_8D62_8E90D5BF7EA4__INCLUDED_)
#define AFX_LOGACTOGGLECUMULATIVEEVENTSTATISTICS_H__3F6DEBA5_BF46_4697_8D62_8E90D5BF7EA4__INCLUDED_


#include "LogConstraintMessage.h"

class CLogacToggleCumulativeEventStatistics : public CLogConstraintMessage    
{
	// ****************************************
	// Construction/Destruction
public:

	CLogacToggleCumulativeEventStatistics(long time, const zString type, bool bAccumulate);

	virtual ~CLogacToggleCumulativeEventStatistics();		// Public so the CLogState can delete messages


	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogacToggleCumulativeEventStatistics& rMsg);

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

	CLogacToggleCumulativeEventStatistics(const CLogacToggleCumulativeEventStatistics& oldMessage);
	CLogacToggleCumulativeEventStatistics& operator=(const CLogacToggleCumulativeEventStatistics& rhs);


	// ****************************************
	// Data members
private:

	const zString m_ACNType;		       // Type of active cell network modified
	const bool	  m_bCumulativeStatistics; // Flag showing if statistics are accumulated (true) or not (false)
};

#endif // !defined(AFX_LOGACTOGGLECUMULATIVEEVENTSTATISTICS_H__3F6DEBA5_BF46_4697_8D62_8E90D5BF7EA4__INCLUDED_)
