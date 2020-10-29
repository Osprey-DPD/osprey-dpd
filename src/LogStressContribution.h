// LogStressContribution.h: interface for the CLogStressContribution class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSTRESSCONTRIBUTION_H__3220E4B3_8B93_4D57_BE20_20B445D3BF18__INCLUDED_)
#define AFX_LOGSTRESSCONTRIBUTION_H__3220E4B3_8B93_4D57_BE20_20B445D3BF18__INCLUDED_


#include "LogInfoMessage.h"

class CLogStressContribution : public CLogInfoMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogStressContribution(long time, const zString stressType, bool bAdded);

	virtual ~CLogStressContribution();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogStressContribution& rMsg);

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

	CLogStressContribution(const CLogStressContribution& oldMessage);
	CLogStressContribution& operator=(const CLogStressContribution& rhs);


	// ****************************************
	// Data members
private:

	const zString m_Type;	// Holds "Bead", "Bond" or "BondPair"

	bool    m_bAdded;		// Flag showing if stress contribution is being added

};

#endif // !defined(AFX_LOGSTRESSCONTRIBUTION_H__3220E4B3_8B93_4D57_BE20_20B445D3BF18__INCLUDED_)


