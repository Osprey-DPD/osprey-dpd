// LogCurrentStateBox.h: interface for the CLogCurrentStateBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCURRENTSTATEBOX_H__61904E20_FD9F_49D8_B2A3_ABED586F8C9D__INCLUDED_)
#define AFX_LOGCURRENTSTATEBOX_H__61904E20_FD9F_49D8_B2A3_ABED586F8C9D__INCLUDED_


#include "LogInfoMessage.h"

class CLogCurrentStateBox : public CLogInfoMessage
{
	// ****************************************
	// Construction/Destruction
public:

	CLogCurrentStateBox(long time, bool bIsBoxDisplayed);

	virtual ~CLogCurrentStateBox();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogCurrentStateBox& rMsg);

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

	CLogCurrentStateBox(const CLogCurrentStateBox& oldMessage);
	CLogCurrentStateBox& operator=(const CLogCurrentStateBox& rhs);


	// ****************************************
	// Data members
private:

	const bool	m_bIsBoxDisplayed;	// Flag showing whether bounding box is displayed

};

#endif // !defined(AFX_LOGCURRENTSTATEBOX_H__61904E20_FD9F_49D8_B2A3_ABED586F8C9D__INCLUDED_)
