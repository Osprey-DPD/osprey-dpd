// epLamella.h: interface for the epLamella class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPLAMELLA_H__1359C267_068B_4973_ACEF_30B4BB25A0D5__INCLUDED_)
#define AFX_EPLAMELLA_H__1359C267_068B_4973_ACEF_30B4BB25A0D5__INCLUDED_


#include "xxProtocol.h"

class epLamella : public xxProtocol  
{
public:
	// ****************************************
	// Construction/Destruction

	epLamella(const zString runId);

	virtual ~epLamella();


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

#endif // !defined(AFX_EPLAMELLA_H__1359C267_068B_4973_ACEF_30B4BB25A0D5__INCLUDED_)
