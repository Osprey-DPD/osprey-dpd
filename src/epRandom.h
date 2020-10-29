// epRandom.h: interface for the epRandom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPRANDOM_H__9D18A5DB_ADA3_4045_9B66_5ABA227649F0__INCLUDED_)
#define AFX_EPRANDOM_H__9D18A5DB_ADA3_4045_9B66_5ABA227649F0__INCLUDED_


#include "xxProtocol.h"

class epRandom : public xxProtocol  
{
public:
	// ****************************************
	// Construction/Destruction

	epRandom(const zString runId);

	virtual ~epRandom();


	// ****************************************
	// Global functions, static member functions and variables
public:

	static const zString GetType();	// Return the protocol name

private:

	static const zString m_Type;	// Identifier used in protocol file

	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

	// Non-static member function to return the (static) protocol name that
	// must be provided by each derived class. A static GetType() function
	// must also be provided.

	virtual const zString GetProtocolType() const;

	virtual bool IsExternalFieldAllowed()	const;
	virtual bool IsExternalFieldRequired()	const;
	virtual bool IsProbeFieldAllowed()		const;
	virtual bool IsProbeFieldRequired()		const;


	// ****************************************
	// Public access functions
public:


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

protected:



private:

	// Default data for the simulation defined in CInputData




};

#endif // !defined(AFX_EPRANDOM_H__9D18A5DB_ADA3_4045_9B66_5ABA227649F0__INCLUDED_)
