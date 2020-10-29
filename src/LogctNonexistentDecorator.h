// LogctNonexistentDecorator.h: interface for the CLogctNonexistentDecorator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGCTNONEXISTENTDECORATOR_H__F9FC3358_F942_483B_9243_06765B5C770E__INCLUDED_)
#define AFX_LOGCTNONEXISTENTDECORATOR_H__F9FC3358_F942_483B_9243_06765B5C770E__INCLUDED_


#include "LogWarningMessage.h"

class CLogctNonexistentDecorator : public CLogWarningMessage  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogctNonexistentDecorator(long time, const zString decLabel, const zString missingDecLabel);

	virtual ~CLogctNonexistentDecorator();

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, const CLogctNonexistentDecorator& rMsg);

	// ****************************************
	// Public access functions
public:

	inline const zString GetDecLabel()		  const {return m_DecLabel;}
	inline const zString GetMissingDecLabel() const {return m_MissingDecLabel;}

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

	// ****************************************
	// Data members
private:


	const zString m_DecLabel;			// label for a decorator instance 
	const zString m_MissingDecLabel;	// label for a required non-existent decorator instance 

};

#endif // !defined(AFX_LOGCTNONEXISTENTDECORATOR_H__F9FC3358_F942_483B_9243_06765B5C770E__INCLUDED_)
