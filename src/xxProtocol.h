// xxProtocol.h: interface for the xxProtocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXPROTOCOL_H__DFA9E84B_57FF_4991_B713_5EDF2B81CF6E__INCLUDED_)
#define AFX_XXPROTOCOL_H__DFA9E84B_57FF_4991_B713_5EDF2B81CF6E__INCLUDED_

// Include file that holds XML-related flags included here to avoid
// every derived class having to explicitly include it.

#include "SimXMLFlags.h"

// Include file for the Protocol Factory object

#include "acfProtocolFactory.h"


#include "xxBase.h"

class xxProtocol : public xxBase  
{
public:
	// ****************************************
	// Construction/Destruction: protected constructor declared below

	virtual ~xxProtocol();


	// ****************************************
	// Global functions, static member functions and variables
public:


	// ****************************************
	// PVFs that must be implemented by all instantiated derived classes 
public:

	virtual const zString GetProtocolType() const = 0;

	virtual bool IsExternalFieldAllowed()	const = 0;
	virtual bool IsExternalFieldRequired()	const = 0;
	virtual bool IsProbeFieldAllowed()		const = 0;
	virtual bool IsProbeFieldRequired()		const = 0;


	// ****************************************
	// Public access functions
public:

	inline const zString GetProtocolId()		   const {return m_RunId;}
	inline const zString GetProtocolDocumentName() const {return m_FileName;}

	inline bool  IsProtocolValid()				   const {return m_bValid;}
	inline  bool IsExternalFieldActive()		   const {return m_bIsExternalFieldActive;}
	inline  bool IsProbeFieldActive()			   const {return m_bIsProbeFieldActive;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor and PVF's ensure that only derived classes can be 
	// instantiated. 

	xxProtocol(const zString name, const zString runId);

	// Helper functions used by derived classes to set protocol data

	void SetValid(bool bValid);

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them private and providing NO definitions.

	xxProtocol(const xxProtocol& oldProtocol);
	xxProtocol& operator=(const xxProtocol& rhs);

	// ****************************************
	// Data members

protected:




private:

	const zString m_Type;			// Acronym for the protocol provided by derived classes
	const zString m_RunId;			// User-suplied identifier for the run
	const zString m_FileName;		// Full name of protocol document

	bool m_bValid;					// Flag showing if the protocol's data is valid
	bool m_bIsProbeFieldActive;		// Flag showing if the probe field is active
	bool m_bIsExternalFieldActive;	// Flag showing if the external field is active

};

#endif // !defined(AFX_XXPROTOCOL_H__DFA9E84B_57FF_4991_B713_5EDF2B81CF6E__INCLUDED_)
