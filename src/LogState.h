// LogState.h: interface for the CLogState class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGSTATE_H__17ED5758_EB6E_4A5E_AA82_036ADEE53173__INCLUDED_)
#define AFX_LOGSTATE_H__17ED5758_EB6E_4A5E_AA82_036ADEE53173__INCLUDED_


// Forward declarations

class CInputData;
class xxMessage;


#include "xxState.h"

class CLogState : public xxState  
{
	// ****************************************
	// Construction/Destruction
public:

	CLogState(const CInputData& rData);

	virtual ~CLogState();

	// ****************************************
	// Global functions, static member functions and variables
public:

	// Static member function to allow any object to add a message 
	// to the CLogState. It calls the member function AddMessage.

	static void StaticAddMessage(xxMessage* const pMsg);

private:

	static CLogState* m_pInstance;		// Pointer to single instance of CLogState class

	// ****************************************
	// PVFs that must be overridden by all derived classes
public:

	bool Serialize();
	
	// Parallel version of the Serialize function
	
	bool SerializeP();

	// ****************************************
	// Public access functions
public:

	long GetMessageTotal();
	long GetUnWrittenMessageTotal();






	// ****************************************
	// Protected local functions
protected:


	// ****************************************
	// Implementation


	// ****************************************
	// Private functions
private:

	// Member function called by StaticAddMessage

	void AddMessage(xxMessage* const pMsg);


	// ****************************************
	// Data members
private:

	MessageSequence m_vMessages;			// Set of all messages
	MessageSequence m_vUnWrittenMessages;	// Set of messages not yet serialized

};

#endif // !defined(AFX_LOGSTATE_H__17ED5758_EB6E_4A5E_AA82_036ADEE53173__INCLUDED_)
