// xxMessage.h: interface for the xxMessage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XXMESSAGE_H__F6CFB3AD_78F2_4D77_9350_9C0D4C5C924F__INCLUDED_)
#define AFX_XXMESSAGE_H__F6CFB3AD_78F2_4D77_9350_9C0D4C5C924F__INCLUDED_

// Include file that holds XML-related flags included here to avoid
// every derived class having to include it explicitly.

#include "SimXMLFlags.h"


#include "xxBase.h"

class xxMessage : public xxBase
{

// ****************************************
// Construction/Destruction: protected constructor declared below
public:

	virtual ~xxMessage();	// Public so the CLogState can delete messages

	// ****************************************
	// Global functions, static member functions and variables
public:

	friend zOutStream& operator<<(zOutStream& os, xxMessage& rMsg);

	static long StaticGetMessageTotal();

private:

	static long m_MessageTotal;		// Total number of created messages

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	virtual	void Serialize(zOutStream& os) const = 0;

	// ****************************************
	// Public access functions
public:

	// Access functions for all xxMessage sub-classes

	inline long GetId()					const {return m_id;}
	inline long GetTime()				const {return m_Time;}
	inline bool IsSerialized()			const {return m_bIsSerialized;}
	inline long GetMessageTotal()		const {return m_MessageTotal;}

	// ****************************************
	// Protected local functions
protected:

	// Protected constructor ensures that only subclasses can be instantiated

	xxMessage(long time);

	// Protected method allows only friend classes to zero the message total

	inline void ZeroMessageTotal() {m_MessageTotal = 0;}

	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Explicitly disallow the copy constructor and assignment operators
	// by declaring them privately but providing NO definitions.

	xxMessage(const xxMessage& oldMessage);
	xxMessage& operator=(const xxMessage& rhs);

	// Helper functions to write the messages to the log file

	// XML output
	zOutStream& putXMLStartTags(zOutStream& os) const;
	zOutStream& putXMLEndTags(zOutStream& os) const;

	// ASCII output
	zOutStream& putASCIIStartTags(zOutStream& os) const;
	zOutStream& putASCIIEndTags(zOutStream& os) const;

	// ****************************************
	// Data members
private:

	const long m_id;				// Unique identifier for a message
	const long m_Time;				// Message creation time

	bool m_bIsSerialized;			// Flag showing if message has already been written

};

#endif // !defined(AFX_XXMESSAGE_H__F6CFB3AD_78F2_4D77_9350_9C0D4C5C924F__INCLUDED_)
