// LogVesicleStalk.h: interface for the CLogVesicleStalk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGVESICLESTALK_H__6D2FC1C8_D421_4A79_8FF6_6D785F6F567B__INCLUDED_)
#define AFX_LOGVESICLESTALK_H__6D2FC1C8_D421_4A79_8FF6_6D785F6F567B__INCLUDED_


#include "LogEventMessage.h"

class CLogVesicleStalk : public CLogEventMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogVesicleStalk(long time, const zString name1, long type1, 
								const zString name2, long type2);
	virtual ~CLogVesicleStalk();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogVesicleStalk& rMsg);

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

	CLogVesicleStalk(const CLogVesicleStalk& oldMessage);
	CLogVesicleStalk& operator=(const CLogVesicleStalk& rhs);

	// ****************************************
	// Data members
private:

	const zString m_Name1;				// Aggregate name, e.g., bilayer
	const long    m_Type1;				// Aggregate id, e.g., 0, 1, 2
	const zString m_Name2;				// Aggregate name, e.g., bilayer
	const long    m_Type2;				// Aggregate id, e.g., 0, 1, 2

};

#endif // !defined(AFX_LOGVESICLESTALK_H__6D2FC1C8_D421_4A79_8FF6_6D785F6F567B__INCLUDED_)
